
#pragma once

#include <variant>


namespace npp::detail {

template <typename T, typename Variant>
struct is_alternative { static const bool value = false; };

template <typename T, typename... V>
struct is_alternative<T, std::variant<V...>> {
    static const bool value = (... || std::is_same_v<T, V>);
};

} // namespace npp::detail


namespace npp {

template <typename T, typename V>
concept variant_alternative = detail::is_alternative<T, V>::value;

} // namespace npp
