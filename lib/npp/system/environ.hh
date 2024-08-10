
#pragma once

#include <memory>

#include "npp/typing/map.hh"


/*
 * Alternatives:
 *   - std::getenv (https://en.cppreference.com/w/cpp/utility/program/getenv)
 */


namespace npp::detail {

/// @warning this class uses a mutex for thread-safe cache management, which is slow and
///          might not be necessary depending on the context of the application
class environment_variables_manager {

public:
    environment_variables_manager(): _environ(nullptr) {}

    environment_variables_manager(const environment_variables_manager&) = delete;
    environment_variables_manager(environment_variables_manager&&)      = delete;

    environment_variables_manager& operator=(const environment_variables_manager&) = delete;
    environment_variables_manager& operator=(environment_variables_manager&&)      = delete;

    std::shared_ptr<const npp::string_map<std::string>> operator()();

    // TODO setenv ?
    // TODO unsetenv ?

    void invalidate();

private:
    std::shared_ptr<npp::string_map<std::string>> _environ;

};

} // namespace npp::detail


namespace npp {

extern npp::detail::environment_variables_manager environ;

} // namespace npp
