
#pragma once

#include <concepts>
#include <string_view>


namespace npp {

template <typename T, class Hash=std::hash<T>>
concept hashable = requires(const T& t) {
    { Hash{}(t) } -> std::same_as<size_t>;
};

} // namespace npp