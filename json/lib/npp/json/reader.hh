
#pragma once

#include <fmt/format.h>

#include "npp/typing/json.hh"


namespace npp {

template <typename T>
concept json_default_convertible = std::integral<T> || std::floating_point<T> || std::same_as<T, std::string>;


template <npp::json Json>
class basic_json_reader {

public:
    basic_json_reader(const Json& data): _data(data) {}

    template <json_default_convertible JsonConvertible>
    void read(std::string_view key, JsonConvertible& value) {
        const auto it = _data.find(key);
        if(it == _data.end()) {
            throw std::runtime_error(fmt::format("Key {} not found", key));
        }
        value = it->template get<JsonConvertible>();
    }


private:
    const Json& _data;

};


using json_reader = basic_json_reader<nlohmann::json>;

} // namespace npp
