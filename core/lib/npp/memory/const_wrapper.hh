
#pragma once

#include <concepts>


namespace npp {

/// @brief This wrapper allows giving const access to an owned object, while still retaining
///        the ability to move (and potentially copy) it
template <std::move_constructible T>
class const_wrapper {
public:
    const_wrapper(T&& obj): _obj(std::move(obj)) {}

    const T* get() const { return &_obj; }

    const T& operator*() const { return _obj; }
    const T* operator->() const { return get(); }

private:
    T _obj;
};

} // namespace npp
