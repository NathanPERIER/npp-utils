
#pragma once

#include <fmt/format.h>

#include "npp/debug.hh"


namespace npp {

struct no_debug {};

template <typename Func>
inline auto in_debug(Func f) {
    if constexpr (debug_mode) {
        return f();
    }
    return no_debug{};
};

} // namespace npp


template <>
struct fmt::formatter<npp::no_debug> {

    constexpr auto parse(fmt::format_parse_context& ctx) -> fmt::format_parse_context::iterator {
        auto it = ctx.begin();
        if (it != ctx.end() && *it != '}') {
            throw fmt::format_error("invalid ndebug format");
        }
        return it;
    }

    auto format(npp::no_debug, fmt::format_context& ctx) const -> fmt::format_context::iterator {
        return fmt::format_to(ctx.out(), "[discarded]");
    }
};
