
#pragma once

#include <concepts>


namespace npp {

template <typename F, typename T>
concept provider = std::invocable<F> && std::convertible_to<std::invoke_result_t<F>, T>;

template <class F, class Res, class... Args>
concept yield_invocable = std::invocable<F, Args...> && std::same_as<std::invoke_result_t<F, Args...>, Res>;

template <class F, class... Args>
concept void_invocable = yield_invocable<F, void, Args...>;

template <class F, class Res, class... Args>
concept yield_convertible_invocable = std::invocable<F, Args...> && std::convertible_to<std::invoke_result_t<F, Args...>, Res>;

} // namespace npp
