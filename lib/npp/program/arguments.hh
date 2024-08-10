
#pragma once

#include <deque>
#include <stdexcept>
#include <string>
#include <vector>


namespace npp {

class program_args {

public:
    program_args(): _args() {}
    program_args(std::initializer_list<std::string> args): _args(args) {}
    program_args(int argc, char** argv, bool drop_first = true): _args() {
        for(int i = (drop_first ? 1 : 0); i < argc; i++) {
            _args.emplace_back(*(argv + i));
        }
    }

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

    size_t size() const { return _args.size(); }
    size_t argc() const { return size();       }

    bool empty() const { return size() == 0; }

    /// @warning any modification on the object will invalidate the returned pointer, also
    ///          modifying the returned pointer might cause undefined behaviours
    char** argv() {
        if(_cache.size() > 0) {
            return _cache.data();
        }

        for(std::string& arg: _args) {
            // This is terribly bad, is there a better way ?
            _cache.push_back(const_cast<char*>(arg.c_str()));
        }
        _cache.push_back(nullptr);

        return _cache.data();
    }

    bool operator==(const program_args& args) const {
        return _args == args._args;
    }

private:
    std::deque<std::string> _args;
    mutable std::vector<char*> _cache;

    void clear_cache() {
        if(_cache.size() > 0) {
            _cache.clear();
        }
    }

};

} // namespace npp
