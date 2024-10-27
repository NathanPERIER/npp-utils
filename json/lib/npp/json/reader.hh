
#pragma once

#include <fmt/format.h>

#include "npp/error.hh"
#include "npp/json/convert_base.hh"
#include "npp/typing/functional.hh"


namespace npp {

template <typename F, typename Json, typename T>
concept json_convert_func = npp::json<Json> && std::invocable<F, const Json&> && std::convertible_to<std::invoke_result_t<F, const Json&>, T>;


template <npp::json Json>
class basic_json_reader {

public:
    basic_json_reader(const Json& data): _data(data) {}


    template <typename T, json_convert_func<Json, T> Conv>
    basic_json_reader<Json>& read(std::string_view key, T& value, Conv converter) {
        read_internal<T, false, false, Conv>(key, value, std::move(converter), []() -> T { throw unreachable(); });
        return *this;
    }

    template <typename T, json_convert_func<Json, T> Conv>
    basic_json_reader<Json>& read_opt(std::string_view key, T& value, Conv converter) {
        read_internal<T, true, false, Conv>(key, value, std::move(converter), []() -> T { throw unreachable(); });
        return *this;
    }

    template <typename T, json_convert_func<Json, T> Conv> requires(std::copy_constructible<T>)
    basic_json_reader<Json>& read_opt(std::string_view key, T& value, Conv converter, const T& default_value) {
        read_internal<T, true, true, Conv>(key, value, std::move(converter), [&default_value]() -> T { return default_value; });
        return *this;
    }

    template <typename T, json_convert_func<Json, T> Conv, npp::provider<T> Provider>
    basic_json_reader<Json>& read_opt(std::string_view key, T& value, Conv converter, Provider get_default) {
        read_internal<T, true, true, Conv, Provider>(key, value, std::move(converter), std::move(get_default));
        return *this;
    }


    template <json_convertible<Json> JsonConvertible>
    basic_json_reader<Json>& read(std::string_view key, JsonConvertible& value) {
        return read<JsonConvertible>(key, value, [](const Json& data) -> JsonConvertible { return npp::convert_json<JsonConvertible>(data); });
    }

    template <json_convertible<Json> JsonConvertible>
    basic_json_reader<Json>& read_opt(std::string_view key, JsonConvertible& value) {
        return read_opt<JsonConvertible>(key, value, [](const Json& data) -> JsonConvertible { return npp::convert_json<JsonConvertible>(data); });
    }

    template <json_convertible<Json> JsonConvertible> requires(std::copy_constructible<JsonConvertible>)
    basic_json_reader<Json>& read_opt(std::string_view key, JsonConvertible& value, const JsonConvertible& default_value) {
        return read_opt<JsonConvertible>(key, value, [](const Json& data) -> JsonConvertible { return npp::convert_json<JsonConvertible>(data); }, default_value);
    }

    template <json_convertible<Json> JsonConvertible, npp::provider<JsonConvertible> Provider>
    basic_json_reader<Json>& read_opt(std::string_view key, JsonConvertible& value, Provider get_default) {
        return read_opt<JsonConvertible>(key, value, [](const Json& data) -> JsonConvertible { return npp::convert_json<JsonConvertible>(data); }, std::move(get_default));
    }


private:
    const Json& _data;

    template <typename T, bool Optional, bool UseDefault, json_convert_func<Json, T> Conv, npp::provider<T> Provider>
    void read_internal(std::string_view key, T& value, Conv converter, Provider get_default) {
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
        value = converter(*it);
    }

};


using json_reader = basic_json_reader<nlohmann::json>;

} // namespace npp
