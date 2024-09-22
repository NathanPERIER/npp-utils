
#pragma once

#include <concepts>
#include <string_view>


namespace npp {

template <typename T>
concept hashable = requires(const T& t) {
    { std::hash<T>{}(t) } -> std::same_as<size_t>;
};

} // namespace npp