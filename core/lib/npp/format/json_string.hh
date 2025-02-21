
#pragma once

#include <tuple>

#include "npp/math/bitwise.hh"
#include "npp/string/lazy.hh"


namespace npp {

std::string parse_json_string(std::string_view buf, size_t pos = 0);

npp::lazy_string parse_json_string_lazy(std::string_view buf, size_t pos = 0);


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
        size_t begin = 0;
        size_t pos = 0;
        while(pos < str.sv.size()) {
            const char current_char = str.sv[pos];
            if(current_char == '"' || current_char == '\\') {
                fmt::format_to(ctx.out(), "{}\\", str.sv.substr(begin, pos - begin));
                begin = pos;
                pos++;
                continue;
            }
            if(std::isprint(static_cast<unsigned char>(current_char))) {
                pos++;
                continue;
            }
            const uint8_t leading_ones = 7 - npp::log2(~static_cast<uint8_t>(current_char));
            if(leading_ones > 0) {
                pos += leading_ones;
                continue;
            }
            if(pos > begin) {
                fmt::format_to(ctx.out(), "{}", str.sv.substr(begin, pos - begin));
            }
            switch(current_char) {
                case '\n': fmt::format_to(ctx.out(), "\\n"); break;
                case '\t': fmt::format_to(ctx.out(), "\\t"); break;
                case '\r': fmt::format_to(ctx.out(), "\\r"); break;
                default:   fmt::format_to(ctx.out(), "\\u00{:0>2x}", current_char);
            }
            pos++;
            begin = pos;
        }
        if(begin < str.sv.size() && pos > begin) {
            fmt::format_to(ctx.out(), "{}", str.sv.substr(begin));
        }
        return fmt::format_to(ctx.out(), "\"");
    }
};
