
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

    template <typename T, json_convert_func<Json, T> Conv, typename V>
    requires(std::constructible_from<T, const V&>)
    basic_json_reader<Json>& read_opt(std::string_view key, T& value, Conv converter, const V& default_value) {
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

    template <json_convertible<Json> T, typename V>
    requires(std::constructible_from<T, const V&>)
    basic_json_reader<Json>& read_opt(std::string_view key, T& value, const V& default_value) {
        read_internal<T, true, std::nullptr_t>(key, value, nullptr, [&default_value]() -> T { return default_value; });
        return *this;
    }


    template <typename T, typename RecurseFn>
    requires(std::invocable<RecurseFn, basic_json_reader<Json>&, T&>)
    basic_json_reader<Json>& recurse(std::string_view key, T& value, RecurseFn recursor) {
        recurse_internal<false>(key, value, std::move(recursor));
        return *this;
    }

    template <typename T, typename RecurseFn>
    requires(std::invocable<RecurseFn, basic_json_reader<Json>&, T&>)
    basic_json_reader<Json>& recurse_opt(std::string_view key, T& value, RecurseFn recursor) {
        recurse_internal<true>(key, value, std::move(recursor));
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

    template <bool Optional, typename T, typename RecurseFn>
    requires(std::invocable<RecurseFn, basic_json_reader<Json>&, T&>)
    void recurse_internal(std::string_view key, T& value, RecurseFn recursor) {
        const auto it = _data.find(key);
        if(it == _data.end()) {
            if constexpr (!Optional) {
                throw std::runtime_error(fmt::format("Key {} not found", key));
            }
            return;
        }
        recursor(basic_json_reader<Json>(*it), value);
    }

};


using json_reader         = basic_json_reader<nlohmann::json>;
using ordered_json_reader = basic_json_reader<nlohmann::ordered_json>;

} // namespace npp
