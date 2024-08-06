
#pragma once

#include <ranges>

#include "npp/typing/printable.hh"


namespace npp {

template <typename Range>
concept formattable_range = std::ranges::range<Range> && npp::formattable<std::ranges::range_value_t<Range>>;

template <formattable_range Range>
struct joined {
	const Range& items;
	std::string_view separator;
};

} // namespace npp



template <npp::formattable_range Range>
struct fmt::formatter<npp::joined<Range>> {

    constexpr auto parse(fmt::format_parse_context& ctx) -> fmt::format_parse_context::iterator {
        auto it = ctx.begin();
        if (it != ctx.end() && *it != '}') {
            throw fmt::format_error("invalid join format");
        }
        return it;
    }

    auto format(const npp::joined<Range>& j, fmt::format_context& ctx) const -> fmt::format_context::iterator {
        auto it = j.items.begin();
		if(it == j.items.end()) {
			return ctx.out();
		}

		fmt::format_to(ctx.out(), "{}", *it);
		++it;

		while(it != j.items.end()) {
			fmt::format_to(ctx.out(), "{}", j.separator);
			fmt::format_to(ctx.out(), "{}", *it);
			++it;
		}

        return ctx.out();
    }
};


namespace npp {

template <formattable_range Range>
std::string join(const Range& items, std::string_view separator) {
	return fmt::format("{}", joined{ items, separator });
}

template <npp::formattable T>
std::string join(std::initializer_list<T> items, std::string_view separator) {
	const std::vector<T> vec(std::move(items));
	return join(vec, separator);
}

} // namespace npp

