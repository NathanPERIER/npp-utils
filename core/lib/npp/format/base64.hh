
#pragma once

#include <cstdint>
#include <span>
#include <string>
#include <vector>

#include <fmt/format.h>


namespace npp {

std::string base64_encode(std::span<const uint8_t> data, bool padding);
std::string base64url_encode(std::span<const uint8_t> data, bool padding);

inline std::string base64_encode(std::span<const char> data, bool padding) {
    return base64_encode(std::span<const uint8_t>(reinterpret_cast<const uint8_t*>(data.data()), data.size()), padding);
}
inline std::string base64url_encode(std::span<const char> data, bool padding) {
    return base64url_encode(std::span<const uint8_t>(reinterpret_cast<const uint8_t*>(data.data()), data.size()), padding);
}

struct b64_enc {
    std::string_view data;
};
struct b64url_enc {
    std::string_view data;
};


std::vector<uint8_t> base64_decode(std::string_view data);
std::vector<uint8_t> base64url_decode(std::string_view data);

} // namespace npp


template <>
struct fmt::formatter<npp::b64_enc> {
    constexpr auto parse(fmt::format_parse_context& ctx) -> fmt::format_parse_context::iterator {
        auto it = ctx.begin();
        if (it != ctx.end() && *it != '}') {
            throw fmt::format_error("invalid base64 format");
        }
        return it;
    }

    auto format(npp::b64_enc in, fmt::format_context& ctx) const -> fmt::format_context::iterator {
        const std::string encoded = npp::base64_encode(in.data, true);
        return fmt::format_to(ctx.out(), "{}", encoded);
    }
};

template <>
struct fmt::formatter<npp::b64url_enc> {
    constexpr auto parse(fmt::format_parse_context& ctx) -> fmt::format_parse_context::iterator {
        auto it = ctx.begin();
        if (it != ctx.end() && *it != '}') {
            throw fmt::format_error("invalid base64url format");
        }
        return it;
    }

    auto format(npp::b64url_enc in, fmt::format_context& ctx) const -> fmt::format_context::iterator {
        const std::string encoded = npp::base64url_encode(in.data, true);
        return fmt::format_to(ctx.out(), "{}", encoded);
    }
};
