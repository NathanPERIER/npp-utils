
#pragma once

#include <filesystem>

#include <fmt/format.h>

/*
 * Alternatives:
 *   - boost::process (https://www.boost.org/doc/libs/1_86_0/doc/html/process.html)
 */


namespace npp {

/// @brief retrieves the path to the executable of a given process identified by its PID
inline std::filesystem::path proc_exe(pid_t pid) {
    return std::filesystem::canonical(
        std::filesystem::path(fmt::format("/proc/{}/exe", pid))
    );
}

/// @brief retrieves the working directory of a given process identified by its PID
inline std::filesystem::path proc_cwd(pid_t pid) {
    return std::filesystem::canonical(
        std::filesystem::path(fmt::format("/proc/{}/cwd", pid))
    );
}

/// @brief retrieves the root directory of a given process identified by its PID
/// @note this can be different of "/" if the process is launched in a chroot
inline std::filesystem::path proc_root(pid_t pid) {
    return std::filesystem::canonical(
        std::filesystem::path(fmt::format("/proc/{}/root", pid))
    );
}

} // namespace npp
