
#pragma once

#include <fmt/format.h>


namespace npp {

struct csv_field {
    std::string_view data;
};

} // namespace npp


template <>
struct fmt::formatter<npp::csv_field> {
    static constexpr char quote = '"'; 

    constexpr auto parse(fmt::format_parse_context& ctx) -> fmt::format_parse_context::iterator {
        auto it = ctx.begin();
        if (it != ctx.end() && *it != '}') {
            throw fmt::format_error("invalid CSV field format");
        }
        return it;
    }

    auto format(npp::csv_field field, fmt::format_context& ctx) const -> fmt::format_context::iterator {
        fmt::format_to(ctx.out(), "{}", quote);
        size_t begin = 0;
        size_t end = field.data.find(quote);
        while(true) {
            if(end == std::string_view::npos) {
                fmt::format_to(ctx.out(), "{}", field.data.substr(begin));
                break;
            }
            end++;
            fmt::format_to(ctx.out(), "{}{}", field.data.substr(begin, end - begin), quote);
            if(end >= field.data.length()) {
                break;
            }
            begin = end;
            end = field.data.find(quote, begin);
        }
        return fmt::format_to(ctx.out(), "{}", quote);
    }
};
