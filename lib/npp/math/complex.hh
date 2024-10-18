
#pragma once

#include <cmath>
#include <stdexcept>

#include <fmt/format.h>

#include "npp/math/vec2.hh"


namespace npp {

class complex {

public:
    constexpr complex(double re): _real(re), _imaginary(0) {}
    constexpr complex(double re, double im): _real(re), _imaginary(im) {}

    static constexpr complex polar(double modulus, double argument) {
        return complex(modulus * std::cos(argument), modulus * std::sin(argument));
    }

    constexpr double re() const { return _real; }
    constexpr double im() const { return _imaginary; }

    double& re() { return _real; }
    double& im() { return _imaginary; }

    double modulus() const { return std::sqrt(_real * _real + _imaginary * _imaginary); }
    double argument() const { return std::atan2(_imaginary, _real); }

    vec2<R> vec() const { return vec2<R>(_real, _imaginary); }


    constexpr complex operator+(const complex c) const {
        return complex(_real + c._real, _imaginary + c._imaginary);
    }

    constexpr complex operator-(const complex c) const {
        return complex(_real - c._real, _imaginary - c._imaginary);
    }

    constexpr complex operator*(const complex c) const {
        //   (a + ib) * (c + id)
        // = ac + ibc + iad - bd
        // = ac - bd + i(bc + ad)
        return complex(
            _real * c._real - _imaginary * c._imaginary,
            _real * c._imaginary + _imaginary * c._real
        );
    }

    constexpr complex operator/(const complex c) const {
        //   (a + ib) / (c + id)
        // = ((a + ib)(c - id)) / ((c + id)(c - id))
        // = (ac + bd + i(bc - ad)) / (c² + d²)
        // = ((ac + bd)/(c² + d²)) + i((bc - ad)/(c² + d²))
        const double den = c._real * c._real + c._imaginary * c._imaginary;
        if(den == 0) {
            throw std::domain_error("complex division by zero");
        }
        return complex(
            (_real * c._real + _imaginary * c._imaginary) / den,
            (_real * c._imaginary - _imaginary * c._real) / den
        );
    }


    complex& operator+=(const complex c) {
        *this = (*this) + c;
        return *this;
    }

    complex& operator-=(const complex c) {
        *this = (*this) - c;
        return *this;
    }

    complex& operator*=(const complex c) {
        *this = (*this) * c;
        return *this;
    }

    complex& operator/=(const complex c) {
        *this = (*this)/c;
        return *this;
    }


    constexpr bool operator==(const complex c) const {
        return (_real == c._real) && (_imaginary == c._imaginary);
    }

private:
    double _real;
    double _imaginary;

};

constexpr complex operator+(double d, complex c) { return c + d; }
constexpr complex operator*(double d, complex c) { return c * d; }
constexpr complex operator-(double d, complex c) { return complex(d, 0) - c; }
constexpr complex operator/(double d, complex c) { return complex(d, 0) / c; }


struct C {
    using element_type = complex;
    static constexpr element_type zero() { return complex(0, 0); }
    static constexpr element_type one() { return complex(1, 0); }
    static constexpr element_type i() { return complex(0, 1); }

};

} // namespace npp


namespace npp::literals {

constexpr npp::complex operator""_i(unsigned long long im) {
    return npp::complex(0, im);
}

constexpr npp::complex operator""_i(long double im) {
    return npp::complex(0, im);
}

} // namespace npp::literals


template <>
struct fmt::formatter<npp::complex> {

    constexpr auto parse(fmt::format_parse_context& ctx) -> fmt::format_parse_context::iterator {
        if (ctx.begin() != ctx.end() && *ctx.begin() != '}') {
            throw fmt::format_error("invalid complex format");
        }
        return ctx.begin();
    }

    auto format(const npp::complex& c, fmt::format_context& ctx) const -> fmt::format_context::iterator {
        if(c.im() < 0) {
            return fmt::format_to(ctx.out(), "{} - {}i", c.re(), -1 * c.im());
        }
        return fmt::format_to(ctx.out(), "{} + {}i", c.re(), c.im());
    }
};
