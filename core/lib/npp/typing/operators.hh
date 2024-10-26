
#pragma once

#include <concepts>


namespace npp {

template <typename T>
concept consistent_addition = requires(T a, T b) {
    { a + b } -> std::same_as<T>;
};

template <typename T>
concept consistent_substraction = requires(T a, T b) {
    { a - b } -> std::same_as<T>;
};

template <typename T>
concept consistent_multiplication = requires(T a, T b) {
    { a * b } -> std::same_as<T>;
};

template <typename T>
concept consistent_division = requires(T a, T b) {
    { a / b } -> std::same_as<T>;
};

} // namespace npp
