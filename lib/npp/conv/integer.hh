
#pragma once

#include <charconv>
#include <stdexcept>
#include <string>

#include "npp/conv/core.hh"


namespace npp {

template <std::integral Integer>
requires (!std::same_as<Integer, bool>)
struct conversion<std::string_view, Integer> {
    Integer operator()(const std::string_view repr) const {
        if(std::is_same_v<bool, Integer>) {
            if(repr == "true") {
                return true;
            }
            if(repr == "false") {
                return false;
            }
            throw std::runtime_error("Bad boolean");
        }
        Integer res;
        const std::from_chars_result conv_ret = std::from_chars(repr.begin(), repr.end(), res);
        if(conv_ret.ec != std::errc{} || conv_ret.ptr != repr.end()) {
            throw std::runtime_error("Bad integer");
        }
        return res;
    }
};

template<>
struct conversion<std::string_view, bool> {
    bool operator()(const std::string_view repr) const {
        if(repr == "true") {
            return true;
        }
        if(repr == "false") {
            return false;
        }
        throw std::runtime_error("Bad boolean");
    }
};


template <std::integral Integer>
struct conversion<std::string, Integer> {
    Integer operator()(const std::string& repr) const {
        return npp::conv<std::string_view, Integer>(repr);
    }
};

} // namespace npp
