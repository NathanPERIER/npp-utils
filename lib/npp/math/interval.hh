
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

    std::optional<int> lower_bound()  const { return _lb; }
    std::optional<int> higher_bound() const { return _hb; }

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


template <>
struct fmt::formatter<npp::interval> {

    constexpr auto parse(fmt::format_parse_context& ctx) -> fmt::format_parse_context::iterator {
        auto it = ctx.begin();
        if (it != ctx.end() && *it != '}') {
            throw fmt::format_error("invalid interval format");
        }
        return it;
    }

    auto format(const npp::interval& itv, fmt::format_context& ctx) const -> fmt::format_context::iterator {
        fmt::format_to(ctx.out(), "[");
        if(itv.lower_bound().has_value()) {
            fmt::format_to(ctx.out(), "{}", itv.lower_bound().value());
        } else {
            fmt::format_to(ctx.out(), "-inf");
        }
        fmt::format_to(ctx.out(), ", ");
        if(itv.higher_bound().has_value()) {
            fmt::format_to(ctx.out(), "{}", itv.higher_bound().value());
        } else {
            fmt::format_to(ctx.out(), "+inf");
        }
        return fmt::format_to(ctx.out(), "]");
    }
};
