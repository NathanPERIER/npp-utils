
#pragma once

#include "npp/typing/json.hh"


namespace npp {

template <typename T>
concept json_trivially_convertible = std::integral<T> || std::floating_point<T> || std::same_as<T, std::string>;


template <typename T>
struct json_converter;

template <json_trivially_convertible JsonConvertible>
struct json_converter<JsonConvertible> {
    template <npp::json Json>
    static JsonConvertible convert(const Json& data) {
        return data.template get<JsonConvertible>();
    }
};


template <typename T, typename Json>
concept json_convertible = npp::json<Json> && requires(const Json& data) {
    { json_converter<T>::convert(data) } -> std::convertible_to<T>;
};

} // namespace npp
