
#pragma once

#include <concepts>


namespace npp {

template <typename F, typename T>
concept provider = std::invocable<F> && std::convertible_to<std::invoke_result_t<F>, T>;

} // namespace npp
