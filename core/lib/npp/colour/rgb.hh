
#pragma once

#include <cstdint>
#include <string_view>

#include <fmt/format.h>


namespace npp {

struct rgb {
    uint8_t r;
    uint8_t g;
    uint8_t b;

    bool operator==(const rgb& col) const {
        return (r == col.r) && (g == col.g) && (b == col.b);
    }

    static rgb parse(std::string_view repr);
};

struct rgba {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;

    bool operator==(const rgba& col) const {
        return (r == col.r) && (g == col.g) && (b == col.b) && (a == col.a);
    }

    static rgba parse(std::string_view repr);
};

} // namespace npp


template <>
struct fmt::formatter<npp::rgb> {

    constexpr auto parse(fmt::format_parse_context& ctx) -> fmt::format_parse_context::iterator {
        if (ctx.begin() != ctx.end() && *ctx.begin() != '}') {
            throw fmt::format_error("invalid RGB colour format");
        }
        return ctx.begin();
    }

    auto format(const npp::rgb& col, fmt::format_context& ctx) const -> fmt::format_context::iterator {
        return fmt::format_to(ctx.out(), "#{:02x}{:02x}{:02x}", col.r, col.g, col.b);
    }
};

template <>
struct fmt::formatter<npp::rgba> {

    constexpr auto parse(fmt::format_parse_context& ctx) -> fmt::format_parse_context::iterator {
        if (ctx.begin() != ctx.end() && *ctx.begin() != '}') {
            throw fmt::format_error("invalid RGBA colour format");
        }
        return ctx.begin();
    }

    auto format(const npp::rgba& col, fmt::format_context& ctx) const -> fmt::format_context::iterator {
        return fmt::format_to(ctx.out(), "#{:02x}{:02x}{:02x}{:02x}", col.r, col.g, col.b, col.a);
    }
};
