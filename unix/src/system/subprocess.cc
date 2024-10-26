#include "npp/system/subprocess.hh"

#include <cassert>

#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

#include <fmt/format.h>

#include "npp/memory/finally.hh"
#include "npp/system/path.hh"


namespace {

std::filesystem::path executable_path_from_name(std::string_view prog_name) {
    if(prog_name.find('/') == std::string_view::npos) {
        std::optional<std::filesystem::path> opt_path = npp::program_path(prog_name);
        if(!opt_path.has_value()) {
            throw std::runtime_error(fmt::format("Could not find program {} in PATH", prog_name));
        }
        return opt_path.value();
    }

    std::filesystem::path res = std::filesystem::absolute(prog_name).lexically_normal();
    const std::filesystem::file_status st = std::filesystem::status(res);
    if(st.type() != std::filesystem::file_type::regular) {
        throw std::runtime_error(fmt::format("Invalid program path: {}", prog_name));
    }
    return res;
}

void close_opt_fd(std::optional<int>& opt_fd) {
    if(!opt_fd.has_value()) {
        return;
    }
    const int fd = opt_fd.value();
    opt_fd = std::nullopt;
    const int close_st = ::close(fd);
    if(close_st != 0) {
        // Retry on EINTR ? (see manpage)
        fmt::print("NPP: Got error {} while calling close(2) on file descritptor {}\n", errno, fd);
    }
}

} // anonymous namespace


namespace npp::detail {

pipe make_pipe() {
    int fd[2];
    const int pipe_st = ::pipe(fd);
    if(pipe_st != 0) {
        throw std::runtime_error(fmt::format("Got error {} while calling pipe(2)", errno));
    }
    return pipe(fd[0], fd[1]);
}

void pipe::close_read()  { ::close_opt_fd(_read_fd);  }
void pipe::close_write() { ::close_opt_fd(_write_fd); }

} // namespace npp::detail



namespace npp {

template <bool Block>
bool subprocess::wait() {
    if(_exit_status.has_value()) {
        return true;
    }

    int status;
    constexpr int wait_options = (Block ? 0 : WNOHANG);
    const pid_t wait_st = waitpid(_pid, &status, wait_options);
    if constexpr(!Block) {
        if(wait_st == 0) { return false; }
    }
    if(wait_st == -1) {
        throw std::runtime_error(fmt::format("Got error {} while calling waitpid(2)", errno));
    }
    if(wait_st != _pid) {
        throw std::runtime_error(fmt::format("Bad PID returned by waitpid(2) (got {}, expected {})", wait_st, _pid));
    }

    _exit_status = status;
    return true;
}

void subprocess::join() { assert(wait<true>()); }

bool subprocess::poll_stopped() { return wait<false>(); }

std::optional<int> subprocess::retcode() {
    if(!_exit_status.has_value() || !WIFEXITED(_exit_status.value())) {
        return std::nullopt;
    }
    return WEXITSTATUS(_exit_status.value());
}

std::optional<int> subprocess::termsig() {
    if(!_exit_status.has_value() || !WIFSIGNALED(_exit_status.value())) {
        return std::nullopt;
    }
    return WTERMSIG(_exit_status.value());
}



void subprocess::signal(int sig) {
    const int kill_st = kill(_pid, sig);
    if(kill_st != 0) {
        throw std::runtime_error(fmt::format("Got error {} while calling kill(2)", errno));
    }
}

subprocess subprocess::spawn(std::string_view prog_name, program_args& args) {
    args.set_executable(prog_name);
    const std::filesystem::path exec_path = ::executable_path_from_name(prog_name);

    detail::pipe ret_pipe = detail::make_pipe();

    const pid_t pid = vfork();
    if(pid == -1) {
        throw std::runtime_error(fmt::format("Got error {} while calling vfork(2)", errno));
    }

    // TODO pass stdin/stdout/stderr as pipes with dup2(2)

    if(pid == 0) {
        detail::pipe ret_pipe_shr = ret_pipe.share();
        ret_pipe_shr.close_read();
        execve(exec_path.string().c_str(), args.argv(), environ);
        const int err = errno;
        const auto do_exit = npp::finally([err]() { exit(err); });
        while(::write(ret_pipe_shr.write_fd().value(), &err, sizeof(int)) == -1 && errno == EINTR);
        ret_pipe_shr.close_write();
    }

    ret_pipe.close_write();
    int err;
    if(::read(ret_pipe.read_fd().value(), &err, sizeof(int)) > 0) {
        ret_pipe.close_read();
        throw std::runtime_error(fmt::format("Got error {} while calling execve(2)", errno));
    }
    ret_pipe.close_read();

    return subprocess(exec_path, pid);
}

} // namespace npp

