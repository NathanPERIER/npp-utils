
#pragma once

#include <concepts>
#include <ostream>

#include <fmt/format.h>


namespace npp {

template <typename T, typename Stream = std::ostream>
concept streamable = requires(Stream& os, T value) {
    { os << value } -> std::convertible_to<Stream&>;
};

template <typename T>
concept formattable = fmt::is_formattable<T>::value;

} // namespace npp
