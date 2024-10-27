
#pragma once

#include <fmt/format.h>

#include "npp/error.hh"
#include "npp/json/convert_base.hh"
#include "npp/typing/functional.hh"


namespace npp {

template <npp::json Json>
class basic_json_reader {

public:
    basic_json_reader(const Json& data): _data(data) {}

    template <json_convertible<Json> JsonConvertible>
    basic_json_reader<Json>& read(std::string_view key, JsonConvertible& value) {
        read_internal<JsonConvertible, false, false>(key, value, []() -> JsonConvertible { throw unreachable(); });
        return *this;
    }

    template <json_convertible<Json> JsonConvertible>
    basic_json_reader<Json>& read_opt(std::string_view key, JsonConvertible& value) {
        read_internal<JsonConvertible, true, false>(key, value, []() -> JsonConvertible { throw unreachable(); });
        return *this;
    }

    template <json_convertible<Json> JsonConvertible> requires(std::copy_constructible<JsonConvertible>)
    basic_json_reader<Json>& read_opt(std::string_view key, JsonConvertible& value, const JsonConvertible& default_value) {
        read_internal<JsonConvertible, true, true>(key, value, [&default_value]() -> JsonConvertible { return default_value; });
        return *this;
    }

    template <json_convertible<Json> JsonConvertible, npp::provider<JsonConvertible> Provider>
    basic_json_reader<Json>& read_opt(std::string_view key, JsonConvertible& value, Provider get_default) {
        read_internal<JsonConvertible, true, true, Provider>(key, value, std::move(get_default));
        return *this;
    }


private:
    const Json& _data;

    template <json_convertible<Json> JsonConvertible, bool Optional, bool UseDefault, npp::provider<JsonConvertible> Provider>
    void read_internal(std::string_view key, JsonConvertible& value, Provider get_default) {
        const auto it = _data.find(key);
        if(it == _data.end()) {
            if constexpr (!Optional) {
                throw std::runtime_error(fmt::format("Key {} not found", key));
            }
            if constexpr (UseDefault) {
                value = get_default();
            }
            return;
        }
        value = npp::convert_json<JsonConvertible>(*it);
    }

};


using json_reader = basic_json_reader<nlohmann::json>;

} // namespace npp
