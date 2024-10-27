
#pragma once

#include <fmt/format.h>

#include "npp/json/convert_base.hh"
#include "npp/typing/functional.hh"
#include "npp/typing/null.hh"


namespace npp {

template <typename F, typename Json, typename T>
concept json_convert_func = npp::json<Json> && std::invocable<F, const Json&> && std::convertible_to<std::invoke_result_t<F, const Json&>, T>;


template <npp::json Json>
class basic_json_reader {

public:
    basic_json_reader(const Json& data): _data(data) {}


    template <typename T, json_convert_func<Json, T> Conv>
    basic_json_reader<Json>& read(std::string_view key, T& value, Conv converter) {
        read_internal<T, false, Conv, std::nullptr_t>(key, value, std::move(converter), nullptr);
        return *this;
    }

    template <typename T, json_convert_func<Json, T> Conv, typename DefaultProvider = nullptr_t>
    requires(npp::provider<DefaultProvider, T> || npp::nulltype<DefaultProvider>)
    basic_json_reader<Json>& read_opt(std::string_view key, T& value, Conv converter, DefaultProvider get_default = nullptr) {
        read_internal<T, true, Conv, std::nullptr_t>(key, value, std::move(converter), std::move(get_default));
        return *this;
    }

    template <typename T, json_convert_func<Json, T> Conv> requires(std::copy_constructible<T>)
    basic_json_reader<Json>& read_opt(std::string_view key, T& value, Conv converter, const T& default_value) {
        read_internal<T, true, Conv>(key, value, std::move(converter), [&default_value]() -> T { return default_value; });
        return *this;
    }


    template <json_convertible<Json> T>
    basic_json_reader<Json>& read(std::string_view key, T& value) {
        read_internal<T, false, std::nullptr_t, std::nullptr_t>(key, value, nullptr, nullptr);
        return *this;
    }

    template <json_convertible<Json> T, typename DefaultProvider = nullptr_t>
    requires(npp::provider<DefaultProvider, T> || npp::nulltype<DefaultProvider>)
    basic_json_reader<Json>& read_opt(std::string_view key, T& value, DefaultProvider get_default = nullptr) {
        read_internal<T, true, std::nullptr_t, DefaultProvider>(key, value, nullptr, std::move(get_default));
        return *this;
    }

    template <json_convertible<Json> T> requires(std::copy_constructible<T>)
    basic_json_reader<Json>& read_opt(std::string_view key, T& value, const T& default_value) {
        read_internal<T, true, std::nullptr_t>(key, value, nullptr, [&default_value]() -> T { return default_value; });
        return *this;
    }


private:
    const Json& _data;

    template <typename T, bool Optional, typename Conv, typename DefaultProvider>
    requires((json_convert_func<Conv, Json, T> || (npp::nulltype<Conv> && json_convertible<T, Json>)) && (npp::provider<DefaultProvider, T> || npp::nulltype<DefaultProvider>))
    void read_internal(std::string_view key, T& value, Conv converter, DefaultProvider get_default) {
        const auto it = _data.find(key);
        if(it == _data.end()) {
            if constexpr (!Optional) {
                throw std::runtime_error(fmt::format("Key {} not found", key));
            }
            if constexpr (!npp::nulltype<DefaultProvider>) {
                value = get_default();
            }
            return;
        }
        if constexpr (npp::nulltype<Conv>) {
            value = npp::convert_json<T>(*it);
        } else {
            value = converter(*it);
        }
    }

};


using json_reader = basic_json_reader<nlohmann::json>;

} // namespace npp
