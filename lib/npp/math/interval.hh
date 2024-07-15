
#pragma once

#include <optional>
#include <stdexcept>

#include <fmt/format.h>


namespace npp {


class interval {

public:
    interval(std::optional<int> lb, std::optional<int> hb): _lb(lb), _hb(hb) {
        if(_lb.has_value() && _hb.has_value() && _lb.value() > _hb.value()) {
            throw std::domain_error(fmt::format("Bad interval : [{}, {}]", _lb.value(), _hb.value()));
        }
    }

    interval(std::pair<std::optional<int>, std::optional<int>> p): interval(p.first, p.second) {}
    interval(std::pair<int, int> p): interval(p.first, p.second) {}


    bool is_finite() const { return _lb.has_value() && _hb.has_value(); }

    std::optional<int> length() const {
        return is_finite() ? std::optional(_hb.value() - _lb.value() + 1) : std::nullopt;
    }

    bool contains(int val) const {
        if(_lb.has_value() && _lb.value() > val) {
            return false;
        }
        if(_hb.has_value() && _hb.value() < val) {
            return false;
        }
        return true;
    }


    bool operator==(const interval& o) {
        return (_lb == o._lb)
            && (_hb == o._hb);
    }

private:
    std::optional<int> _lb;
    std::optional<int> _hb;

};

} // namespace npp



