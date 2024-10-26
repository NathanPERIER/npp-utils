
#pragma once

#include <memory>
#include <vector>

#include "npp/typing/map.hh"


/*
 * Alternatives:
 *   - std::getenv (https://en.cppreference.com/w/cpp/utility/program/getenv)
 */


namespace npp {

class program_env {

public:
    program_env(): _environ() {}
    program_env(char** envp):       program_env(const_cast<const char* const*>(envp)) {}
    program_env(char* const* envp): program_env(const_cast<const char* const*>(envp)) {}
    program_env(const char* const* envp): _environ() {
        for(const char* const* env = envp; (*env) != nullptr; env++) {
            std::shared_ptr<std::string> entry = std::make_shared<std::string>(*env);
            const size_t idx = entry->find('=');
            if(idx == std::string::npos) {
                throw std::runtime_error("Bad environment array");
            }
            _environ.insert({
                entry->substr(0, idx),
                std::pair(
                    (idx < entry->size() - 1) ? std::string_view(*entry).substr(idx+1) : std::string_view(),
                    entry
                )
            });
        }
    }

    program_env(const program_env& env): _environ(env._environ) {}
    program_env& operator=(const program_env& env) {
        _environ = env._environ;
        return *this;
    };

    program_env(program_env&&) = delete;
    program_env& operator=(program_env&&) = delete;

    static program_env from_environ();

    size_t size() const { return _environ.size(); }

    std::optional<std::string_view> get(std::string_view name) const;
    void set(std::string_view name, std::string_view value);
    void unset(std::string_view name);

    char* const* envp() {
        if(_cache.size() == 0) {
            _cache.reserve(_environ.size());
            for(auto& e: _environ) {
                _cache.push_back(e.second.second->data());
            }
            _cache.push_back(nullptr);
        }
        return _cache.data();
    }

private:
    npp::string_map<std::pair<std::string_view, std::shared_ptr<std::string>>> _environ;
    std::vector<char*> _cache;

};

} // namespace npp
