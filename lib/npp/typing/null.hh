
#pragma once

#include <concepts>
#include <cstddef>


namespace npp {

template <typename T>
concept nulltype = std::same_as<std::nullptr_t, T>;

} // namespace npp
