
#pragma once

#include "npp/math/algebraic_ring.hh"


namespace npp {

template <algebraic_ring Ring>
class vec2 {

public:
    using element_type = Ring::element_type;

    vec2(element_type x, element_type y): x(x), y(y) {}

    static vec2<Ring> origin() {
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

} // namespace npp
