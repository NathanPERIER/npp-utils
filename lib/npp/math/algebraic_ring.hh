
#pragma once

#include <concepts>
#include <cstdint>


namespace npp {

struct R {
    using element_type = double;
    static constexpr element_type zero() { return 0; }
    static constexpr element_type one()  { return 1; }
};

struct N {
    using element_type = int64_t;
    static constexpr element_type zero() { return 0; }
    static constexpr element_type one()  { return 1; }
};

template <typename T>
concept algebraic_ring = requires() {
    { T::zero() } -> std::same_as<typename T::element_type>;
    { T::one()  } -> std::same_as<typename T::element_type>;
} && requires(T::element_type a, T::element_type b) {
    { a + b } -> std::same_as<typename T::element_type>;
    { a * b } -> std::same_as<typename T::element_type>;
};

} // namespace npp
