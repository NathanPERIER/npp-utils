
#pragma once

#include <filesystem>
#include <optional>

#include "npp/program/arguments.hh"


/*
 * Alternatives:
 *   - boost::process (https://www.boost.org/doc/libs/1_86_0/doc/html/process.html)
 */


namespace npp::detail {

class pipe {

public:
    pipe();

    pipe(const pipe&) = delete;
    pipe& operator=(const pipe&) = delete;

    std::optional<int> read_fd()  const { return get_fd<0>(); }
    std::optional<int> write_fd() const { return get_fd<1>(); }

    void close_read()  { return close_fd<0>(); }
    void close_write() { return close_fd<1>(); }

    /// @brief used to create a copy of the file descriptors when forking with shared memory
    pipe share() { return pipe(_fd); }

    void close() {
        close_read();
        close_write();
    };

    ~pipe() { close(); }

private:
    int _fd[2];

    pipe(const int fd[2]): _fd{ fd[0], fd[1] } {}

    template <int FdIdx>
    requires(FdIdx == 0 || FdIdx == 1)
    std::optional<int> get_fd() const {
        return _fd[FdIdx] == 0 ? std::nullopt : std::optional(_fd[FdIdx]);
    }

    template <int FdIdx>
    requires(FdIdx == 0 || FdIdx == 1)
    void close_fd();
};

} // namespace npp::detail


namespace npp {

class subprocess {

public:
    subprocess(const subprocess&) = delete;
    subprocess& operator=(const subprocess&) = delete;

    static subprocess spawn(std::string_view prog_name, program_args& args);

    pid_t pid() const { return _pid; }
    const std::filesystem::path& executable() { return _executable; }

    bool stopped() const { return _exit_status.has_value(); }

    std::optional<int> retcode();
    std::optional<int> termsig();

    void join();
    bool poll_stopped();

    void signal(int sig);

private:
    subprocess(const std::filesystem::path& executable, pid_t pid): _executable(executable), _pid(pid) {}

    std::filesystem::path _executable;
    pid_t _pid;
    std::optional<int> _exit_status = std::nullopt;

    template <bool Block>
    bool wait();

};

} // namespace npp

