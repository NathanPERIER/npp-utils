
#pragma once

#include <cctype>

#include <fmt/format.h>


namespace npp {

struct char_ifprint {
    char c;
};

} // namespace npp


template <>
struct fmt::formatter<npp::char_ifprint> {

    constexpr auto parse(fmt::format_parse_context& ctx) -> fmt::format_parse_context::iterator {
        auto it = ctx.begin();
        if (it != ctx.end() && *it != '}') {
            throw fmt::format_error("invalid ifprint char format");
        }
        return it;
    }

    auto format(npp::char_ifprint chr, fmt::format_context& ctx) const -> fmt::format_context::iterator {
        if(std::isprint(static_cast<unsigned char>(chr.c))) {
            return fmt::format_to(ctx.out(), "'{}'", chr.c);
        }
        if(chr.c == '\n') {
            return fmt::format_to(ctx.out(), "\\n");
        }
        if(chr.c == '\t') {
            return fmt::format_to(ctx.out(), "\\t");
        }
        if(chr.c == '\r') {
            return fmt::format_to(ctx.out(), "\\r");
        }
        return fmt::format_to(ctx.out(), "\\x{:x}", static_cast<uint8_t>(chr.c));
    }
};
