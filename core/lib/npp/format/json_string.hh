
#pragma once

#include <cctype>

#include <fmt/format.h>


namespace npp {

struct json_string {
    std::string_view sv;
};

} // namespace npp


template <>
struct fmt::formatter<npp::json_string> {

    constexpr auto parse(fmt::format_parse_context& ctx) -> fmt::format_parse_context::iterator {
        auto it = ctx.begin();
        if (it != ctx.end() && *it != '}') {
            throw fmt::format_error("invalid json string format");
        }
        return it;
    }

    auto format(npp::json_string str, fmt::format_context& ctx) const -> fmt::format_context::iterator {
        fmt::format_to(ctx.out(), "\"");
        throw std::runtime_error("TODO");
        return fmt::format_to(ctx.out(), "\"");
    }
};
