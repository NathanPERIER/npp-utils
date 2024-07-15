
#pragma once

#include <numeric>
#include <stdexcept>
#include <utility>


namespace npp {

class fraction {

public:
    fraction(int num, int den = 1): _num(num), _den(den) {
        if(_den == 0) {
            throw std::domain_error("Fraction denominator cannot be zero");
        }

        if(_num == 0) {
            _den = 1;
            return;
        }

        const int gcd = std::gcd(_num, _den);
        _num = _num / gcd;
        _den = _den / gcd;
    }

    fraction(std::pair<int,int> p): fraction(p.first, p.second) {}


    int num() const { return _num; }
    int den() const { return _den; }

    fraction inv() { return fraction(_den, _num); }

    double compute() { return static_cast<double>(_num) / static_cast<double>(_den); }


    fraction operator*(const fraction& frac) { return fraction(_num * frac._num, _den * frac._den); }
    fraction operator/(const fraction& frac) { return fraction(_num * frac._den, _den * frac._num); }

    fraction operator+(const fraction& frac) {
        const int gcd = std::gcd(_den, frac._den);
        const int den1 = _den / gcd;
        const int den2 = frac._den / gcd;
        return fraction(_num * den2 + frac._num * den1, _den * den2);
    }

    fraction operator-(const fraction& frac) {
        const int gcd = std::gcd(_den, frac._den);
        const int den1 = _den / gcd;
        const int den2 = frac._den / gcd;
        return fraction(_num * den2 - frac._num * den1, _den * den2);
    }

    fraction operator*(int i) { return fraction(_num * i, _den); }
    fraction operator/(int i) { return fraction(_num, _den * i); }

    fraction operator+(int i) { return fraction(_num + i * _den, _den); }
    fraction operator-(int i) { return fraction(_num - i * _den, _den); }


private:
    int _num;
    int _den;

};

} // namespace npp
