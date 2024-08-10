
#pragma once

#include <concepts>


/*
 * Alternatives:
 *   - seastar::defer (https://docs.seastar.io/master/defer_8hh_source.html)
 */


namespace npp {

template <std::invocable F>
class finally {

public:
    finally(F&& f): _func(std::forward<F>(f)) {}

    // Maybe these can actually be useful ? (see Seastar implementation)
    finally(finally<F>&&)               = delete;
    finally<F>& operator=(finally<F>&&) = delete;

    finally(const finally<F>&)               = delete;
    finally<F>& operator=(const finally<F>&) = delete;

    ~finally() { _func(); }

private:
    F _func;

};

} // namespace npp
