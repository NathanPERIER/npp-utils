
#pragma once

#include <fmt/format.h>

#include "npp/json/convert.hh"


namespace npp {

template <npp::json Json>
class basic_json_reader {

public:
    basic_json_reader(const Json& data): _data(data) {}

    template <json_convertible<Json> JsonConvertible>
    basic_json_reader<Json>& read(std::string_view key, JsonConvertible& value) {
        const auto it = _data.find(key);
        if(it == _data.end()) {
            throw std::runtime_error(fmt::format("Key {} not found", key));
        }
        value = json_converter<JsonConvertible>::convert(*it);
        return *this;
    }


private:
    const Json& _data;

};


using json_reader = basic_json_reader<nlohmann::json>;

} // namespace npp
