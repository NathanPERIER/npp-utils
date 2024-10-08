
#pragma once

#include <deque>
#include <stdexcept>
#include <string>
#include <vector>


namespace npp {

class program_args {

public:
    program_args(): _args() {}
    program_args(std::string executable): _executable(std::move(executable)), _args() {}
    program_args(std::string executable, std::initializer_list<std::string> args): _executable(std::move(executable)), _args(args) {}
    program_args(int argc, char** argv):       program_args(argc, const_cast<const char* const*>(argv)) {}
    program_args(int argc, char* const* argv): program_args(argc, const_cast<const char* const*>(argv)) {}
    program_args(int argc, const char* const* argv): _executable((argc >= 1) ? *argv : ""), _args() {
        for(int i = 1; i < argc; i++) {
            _args.emplace_back(*(argv + i));
        }
    }

    program_args(const program_args& args): _executable(args._executable), _args(args._args) { };
    program_args& operator=(const program_args& args) {
        _cache.clear();
        _executable = args._executable;
        _args = args._args;
        return *this;
    };

    program_args(program_args&&) = delete;
    program_args& operator=(program_args&&) = delete;

    void drop() {
        if(_args.empty()) {
            throw std::runtime_error("No arguments to poll");
        }
        _args.pop_front();
        clear_cache();
    }

    bool drop(const std::string_view expected) {
        if(_args.empty()) {
            throw std::runtime_error("No arguments to poll");
        }
        if(_args.front() == expected) {
            _args.pop_front();
            clear_cache();
            return true;
        }
        return false;
    }

    const std::string& peek() const {
        if(_args.empty()) {
            throw std::runtime_error("No arguments to poll");
        }
        return _args.front();
    }

    std::string pop() {
        std::string res = peek();
        _args.pop_front();
        clear_cache();
        return res;
    }

    void push(std::string arg) {
        _args.push_back(std::move(arg));
        clear_cache();
    }

    std::string_view executable() const { return _executable; }
    void set_executable(const std::string_view executable_name) {
        clear_cache();
        _executable = std::string(executable_name);
    }

    size_t size() const { return _args.size(); }
    size_t argc() const { return size() + 1;   }

    bool empty() const { return size() == 0; }

    /// @warning any modification on the object will invalidate the returned pointer, also
    ///          modifying the returned pointer might cause undefined behaviours
    char* const* argv() {
        if(_cache.size() > 0) {
            return _cache.data();
        }

        _cache.push_back(_executable.data());
        for(std::string& arg: _args) {
            _cache.push_back(arg.data());
        }
        _cache.push_back(nullptr);

        return _cache.data();
    }

    bool operator==(const program_args& args) const {
        return (_args       == args._args      )
            && (_executable == args._executable);
    }

private:
    std::string _executable;
    std::deque<std::string> _args;
    std::vector<char*> _cache;

    void clear_cache() {
        if(_cache.size() > 0) {
            _cache.clear();
        }
    }

};

} // namespace npp
