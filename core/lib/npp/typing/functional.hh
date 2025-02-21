
#pragma once

#include <concepts>


namespace npp {

template <typename F, typename T>
concept provider = std::invocable<F> && std::convertible_to<std::invoke_result_t<F>, T>;

template <typename F, typename... Args>
concept void_invocable = std::invocable<F, Args...> && std::same_as<std::invoke_result_t<F, Args...>, void>;

} // namespace npp
