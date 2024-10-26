
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
    pipe(): _read_fd(std::nullopt), _write_fd(std::nullopt) {}

    pipe(const pipe&) = delete;
    pipe& operator=(const pipe&) = delete;

    std::optional<int> read_fd()  const { return _read_fd;  }
    std::optional<int> write_fd() const { return _write_fd; }

    void close_read();
    void close_write();

    /// @brief used to create a copy of the file descriptors when forking with shared memory
    pipe share() { return pipe(_read_fd, _write_fd); }

    void close() {
        close_read();
        close_write();
    };

    ~pipe() { close(); }

private:
    std::optional<int> _read_fd;
    std::optional<int> _write_fd;

    pipe(std::optional<int> read_fd, std::optional<int> write_fd): _read_fd(read_fd), _write_fd(write_fd) {}

    friend pipe make_pipe();
};

pipe make_pipe();

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

