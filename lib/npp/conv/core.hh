
#pragma once

#include <concepts>


/*
 * Alternatives:
 *   - folly::to (https://github.com/facebook/folly/blob/main/folly/docs/Conv.md)
 */


namespace npp {

// Note: improvements to be expected in C++23 with the introduction of static operator()

template <typename From, typename To>
struct conversion;

template <typename From, typename To>
constexpr conversion<From, To> conv {};

template <typename From, typename To>
concept convertible = requires(const From& x) {
    { conv<std::decay_t<From>, To>(x) } -> std::convertible_to<To>;
};


} // namespace npp
