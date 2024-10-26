
#pragma once

#include <concepts>
#include <numeric>
#include <stdexcept>
#include <utility>


namespace npp {

template <std::integral Integer>
requires(!std::same_as<Integer,bool>)
class fraction {

public:
    fraction(Integer num, Integer den = 1): _num(num), _den(den) {
        if(_den == 0) {
            throw std::domain_error("Fraction denominator cannot be zero");
        }

        if(_num == 0) {
            _den = 1;
            return;
        }

        const Integer gcd = std::gcd(_num, _den);
        _num = _num / gcd;
        _den = _den / gcd;

        if constexpr (std::unsigned_integral<Integer>) {
            return;
        }

        if(_den < 0) {
            _den *= -1;
            _num *= -1;
        }
    }

    fraction(std::pair<Integer,Integer> p): fraction(p.first, p.second) {}


    Integer num() const { return _num; }
    Integer den() const { return _den; }

    fraction<Integer> inv() const { return fraction(_den, _num); }

    Integer quotient() const { return _num / _den; }
    double compute() const { return static_cast<double>(_num) / static_cast<double>(_den); }

    Integer floor() const {
        if constexpr (!std::unsigned_integral<Integer>) {
            if(_num < 0) { return (_num + 1 - _den) / _den; }
        }
        return _num / _den;
    }

    Integer ceil() const {
        if constexpr (!std::unsigned_integral<Integer>) {
            if(_num < 0) { return _num / _den; }
        }
        return (_num - 1 + _den) / _den;
    }

    fraction<Integer> operator*(const fraction<Integer>& frac) const { return fraction<Integer>(_num * frac._num, _den * frac._den); }
    fraction<Integer> operator/(const fraction<Integer>& frac) const { return fraction<Integer>(_num * frac._den, _den * frac._num); }

    fraction<Integer> operator+(const fraction<Integer>& frac) {
        const Integer gcd = std::gcd(_den, frac._den);
        const Integer den1 = _den / gcd;
        const Integer den2 = frac._den / gcd;
        return fraction<Integer>(_num * den2 + frac._num * den1, _den * den2);
    }

    fraction<Integer> operator-(const fraction<Integer>& frac) const {
        const Integer gcd = std::gcd(_den, frac._den);
        const Integer den1 = _den / gcd;
        const Integer den2 = frac._den / gcd;
        return fraction<Integer>(_num * den2 - frac._num * den1, _den * den2);
    }


    bool operator==(const fraction<Integer>& frac) const {
        return (_num == frac._num)
            && (_den == frac._den);
    }

private:
    Integer _num;
    Integer _den;

};

} // namespace npp
