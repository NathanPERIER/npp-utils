

#pragma once

#include <filesystem>

#include "npp/json/convert.hh"


namespace npp {

template <>
struct json_converter<std::filesystem::path> {
    template <npp::json Json>
    static std::filesystem::path convert(const Json& data) {
        return std::filesystem::path(json_converter<std::string>::convert(data)).lexically_normal();
    }
};


}; // namespace npp
