
#pragma once

#include <concepts>


namespace npp {

template<typename T, typename ... U>
concept any_of = (std::same_as<T, U> || ...);

} // namespace npp
