
#pragma once

#include <optional>
#include <type_traits>

#include <fmt/format.h>

#include "npp/type_name.hh"
#include "npp/typing/map.hh"


namespace npp {

template <typename E>
requires(std::is_enum_v<E>)
struct enum_serialiser;

template <typename E>
concept serialisable_enum = std::is_enum_v<E> && requires() {
    { enum_serialiser<E>::mapping } -> std::same_as<const std::unordered_map<E, std::string>&>;
};

template <serialisable_enum E>
inline std::optional<E> parse_enum(std::string_view repr) {
    static const string_umap<E> parse_mapping = []() {
        string_umap<E> res;
        for(const auto& p: npp::enum_serialiser<E>::mapping) {
            if(!res.insert({ p.second, p.first }).second) [[unlikely]] {
                throw std::runtime_error(fmt::format("Impossible to reverse {} enum mapping: duplicate representation '{}'", npp::type_name<E>(), p.second));
            }
        }
        return res;
    }();
    const auto it = parse_mapping.find(repr);
    if(it != parse_mapping.end()) {
        return it->second;
    }
    return std::nullopt;
}

} // namespace npp


template <npp::serialisable_enum E>
struct fmt::formatter<E> {
    constexpr auto parse(fmt::format_parse_context& ctx) -> fmt::format_parse_context::iterator {
        auto it = ctx.begin();
        if (it != ctx.end() && *it != '}') {
            throw fmt::format_error("invalid enum format");
        }
        return it;
    }

    auto format(E enum_val, fmt::format_context& ctx) const -> fmt::format_context::iterator {
        const auto it = npp::enum_serialiser<E>::mapping.find(enum_val);
        if(it != npp::enum_serialiser<E>::mapping.end()) [[likely]] {
            return fmt::format_to(ctx.out(), "{}", it->second);
        }
        throw std::runtime_error(fmt::format("Impossible to format {} enum value (check the serializing mapping)", npp::type_name<E>()));
    }
};
