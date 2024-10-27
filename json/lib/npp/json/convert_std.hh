

#pragma once

#include <chrono>
#include <filesystem>

#include "npp/json/convert_base.hh"


namespace npp {

template <>
struct json_converter<std::filesystem::path> {
    template <npp::json Json>
    static std::filesystem::path convert(const Json& data) {
        return std::filesystem::path(json_converter<std::string>::convert(data)).lexically_normal();
    }
};


template <class Rep, std::intmax_t Num, std::intmax_t Denom>
struct json_converter<std::chrono::duration<Rep, std::ratio<Num,Denom>>> {

    using duration_type = std::chrono::duration<Rep, std::ratio<Num,Denom>>;

    template <npp::json Json> requires(json_convertible<Rep, Json>)
    static duration_type convert(const Json& data) {
        return duration_type(json_converter<Rep>::convert(data));
    }
};

}; // namespace npp
