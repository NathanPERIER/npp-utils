#include "npp/program/environ.hh"

#include <cassert>
#include <mutex>
#include <unistd.h>

#include "npp/memory/finally.hh"


namespace {

std::mutex environ_mutex;

} // anonymous namespace


namespace npp::detail {

std::shared_ptr<const npp::string_map<std::string>> environment_variables_manager::operator()() {
    ::environ_mutex.lock();
    const auto holder = npp::finally([]() { ::environ_mutex.unlock(); });

    if(_environ != nullptr) {
        return _environ;
    }

    _environ = std::make_unique<npp::string_map<std::string>>();
    for(char** env = ::environ; (*env) != nullptr; env++) {
        const std::string_view entry(*env);
        const size_t idx = entry.find('=');
        assert(idx != std::string_view::npos);
        _environ->insert({
            std::string(entry.substr(0, idx)),
            std::string((idx < entry.size() - 1) ? entry.substr(idx+1) : std::string_view())
        });
    }

    return _environ;
}


void environment_variables_manager::invalidate() {
    ::environ_mutex.lock();
    _environ = nullptr;
    ::environ_mutex.unlock();
}

} // namespace npp::detail


namespace npp {

npp::detail::environment_variables_manager environ;

} // namespace npp
