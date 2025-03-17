
#pragma once

#include <chrono>


namespace npp::detail {

template <typename T>
struct is_duration: std::false_type {};

template <class Rep, class Period>
struct is_duration<std::chrono::duration<Rep, Period>>: std::true_type {};

} // namespace npp::detail


namespace npp {

template <typename T>
concept duration_type = detail::is_duration<T>::value;

} // namespace npp
