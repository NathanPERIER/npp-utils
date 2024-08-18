#include "npp/program/environ.hh"

#include <unistd.h>

#include <fmt/format.h>


namespace npp {

program_env program_env::from_environ() {
    return program_env(::environ);
}

std::optional<std::string_view> program_env::get(std::string_view name) const {
    const auto it = _environ.find(name);
    if(it == _environ.end()) {
        return std::nullopt;
    }
    return it->second.first;
}

void program_env::set(std::string_view name, std::string_view value) {
    if(name.find('=') != std::string_view::npos) {
        throw std::runtime_error("Environment variable cannot contain the '=' symbol");
    }
    std::shared_ptr<std::string> entry = std::make_shared<std::string>(fmt::format("{}={}", name, value));
    auto val = std::pair(
        (value.size() > 0) ? std::string_view(*entry).substr(name.size()+1) : std::string_view(),
        entry
    );
    auto it = _environ.find(name);
    if(it == _environ.end()) {
        _environ.insert({
            std::string(name),
            std::move(val)
        });
    } else {
        it->second = std::move(val);
    }
}

void program_env::unset(std::string_view name) {
    const auto it = _environ.find(name);
    if(it != _environ.end()) {
        _environ.erase(it);
    }
}

} // namespace npp
