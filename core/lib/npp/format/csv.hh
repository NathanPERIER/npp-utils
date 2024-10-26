
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

        size_t prev_pos = 0;
        size_t pos = field.data.find(quote);
        while(pos != std::string_view::npos) {
            fmt::format_to(ctx.out(), "{}{}", field.data.substr(prev_pos, pos - prev_pos + 1), quote);
            prev_pos = pos + 1;
            if(prev_pos >= field.data.size()) {
                break;
            }
            pos = field.data.find(quote, prev_pos);
        }

        if(prev_pos < field.data.size()) {
            fmt::format_to(ctx.out(), "{}", field.data.substr(prev_pos));
        }

        return fmt::format_to(ctx.out(), "{}", quote);
    }
};
