
#pragma once

#include <memory>
#include <string>

#include <fmt/format.h>


namespace npp {

class lazy_string {

public:
    constexpr lazy_string() {}
    constexpr lazy_string(std::string_view sv): _data(sv) {}
    constexpr lazy_string(const char* c_str): _data(c_str) {}

    lazy_string(std::string s): _holder(std::make_shared<const std::string>(std::move(s))), _data(*_holder) {}

    std::string_view view() const { return _data; }
    operator std::string_view() const { return view(); }

    const std::string_view* operator->() const { return &_data; }

    bool operator==(const lazy_string& lazy) const { return view() == lazy.view(); }

private:
    std::shared_ptr<const std::string> _holder;
    std::string_view _data;

};

} // namespace npp



template <>
struct fmt::formatter<npp::lazy_string> {

    constexpr auto parse(fmt::format_parse_context& ctx) -> fmt::format_parse_context::iterator {
        auto it = ctx.begin();
        if (it != ctx.end() && *it != '}') {
            throw fmt::format_error("invalid lazy string format");
        }
        return it;
    }

    auto format(const npp::lazy_string& lazy, fmt::format_context& ctx) const -> fmt::format_context::iterator {
		return fmt::format_to(ctx.out(), "{}", lazy.view());
    }
};
