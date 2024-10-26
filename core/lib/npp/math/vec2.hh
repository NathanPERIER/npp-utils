
#pragma once

#include "npp/math/algebraic_ring.hh"
#include "npp/typing/operators.hh"


namespace npp {

template <algebraic_ring Ring>
class vec2 {

public:
    using element_type = Ring::element_type;

    vec2(element_type x, element_type y): x(x), y(y) {}

    static vec2<Ring> zero() {
        return vec2(Ring::zero(), Ring::zero());
    }

    element_type x;
    element_type y;

    vec2<Ring> operator+(const vec2<Ring>& v) const {
        return vec2<Ring>(x + v.x, y + v.y);
    }

    vec2<Ring> operator+(const element_type& e) const {
        return vec2<Ring>(x + e, y + e);
    }

    vec2<Ring> operator*(const element_type& e) const {
        return vec2<Ring>(x * e, y * e);
    }

    element_type operator*(const vec2<Ring>& v) const {
        return x * v.x + y * v.y;
    }

    bool operator==(const vec2<Ring>& v) const {
        return (x == v.x) && (y == v.y);
    }
};

template <algebraic_ring Ring>
requires(consistent_substraction<typename Ring::element_type>)
vec2<Ring> operator-(const vec2<Ring>& v1, const vec2<Ring>& v2) {
    return vec2<Ring>(v1.x - v2.x, v1.y - v2.y);
}

template <algebraic_ring Ring>
requires(consistent_substraction<typename Ring::element_type>)
vec2<Ring> operator-(const vec2<Ring>& v, const typename Ring::element_type& e) {
    return vec2<Ring>(v.x - e, v.y - e);
}

template <algebraic_ring Ring>
requires(consistent_division<typename Ring::element_type>)
vec2<Ring> operator/(const vec2<Ring>& v, const typename Ring::element_type& e) {
    return vec2<Ring>(v.x / e, v.y / e);
}

} // namespace npp
