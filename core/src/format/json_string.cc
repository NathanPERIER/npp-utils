#include "npp/format/json_string.hh"

#include <stdexcept>

#include <fmt/format.h>

#include "npp/format/hexdump.hh"


namespace {

struct utf16_surrogate_pair {
    unsigned int high: 10 = 0;
    unsigned int low: 10 = 0;
};

void encode_utf16_codepoint_to_utf8(const utf16_surrogate_pair& pair, std::string& res) {
    // pair: uuzzzzzzyy yyyyxxxxxx <=> codepoint - 0x10000
    const uint16_t corrected = static_cast<uint16_t>(pair.high >> 2) + 0b10000;
    res.push_back(0b11110000 | static_cast<char>((corrected >> 6) & 0b00000011));
    res.push_back(0b10000000 | static_cast<char>(corrected & 0x003f));
    res.push_back(0b10000000 | static_cast<char>((pair.high & 0x0003) << 4) | static_cast<char>((pair.low >> 6) & 0x000f));
    res.push_back(0b10000000 | static_cast<char>(pair.low & 0x003f));
}

void encode_utf16_codepoint_to_utf8(uint16_t codepoint, std::string& res) {
    if(codepoint <= 0x007f) {
        res.push_back(static_cast<char>(codepoint & 0x00ff));
        return;
    }
    if(codepoint <= 0x07ff) {
        res.push_back(0b11000000 | static_cast<char>((codepoint >> 6) & 0b00011111));
        res.push_back(0b10000000 | static_cast<char>(codepoint & 0x003f));
        return;
    }
    res.push_back(0b11100000 | static_cast<char>((codepoint >> 12) & 0b00001111));
    res.push_back(0b10000000 | static_cast<char>((codepoint >> 6) & 0x003f));
    res.push_back(0b10000000 | static_cast<char>(codepoint & 0x003f));
}

uint16_t load_utf16_hex_sequence(std::string_view buf, size_t pos) {
    return static_cast<uint16_t>(npp::load_hex_byte(buf[pos], buf[pos+1])) << 8 | npp::load_hex_byte(buf[pos+2], buf[pos+3]);
}

void utf16_escape_sequance_at(std::string_view buf, size_t& pos, std::string& res) {
    static const uint16_t high_surrogate_range_begin = 0xd800;
    static const uint16_t high_surrogate_range_end = 0xdbff;
    static const uint16_t low_surrogate_range_begin = 0xdc00;
    static const uint16_t low_surrogate_range_end = 0xdfff;
    static const uint16_t surrogate_byte_mask = 0x03ff;
    if(pos+4 >= buf.size()) {
        throw std::runtime_error("Incomplete UTF-16 escape sequence");
    }
    const uint16_t utf16 = load_utf16_hex_sequence(buf, pos+1);
    pos += 4;
    if(utf16 >= low_surrogate_range_begin && utf16 <= low_surrogate_range_end) {
        throw std::runtime_error(fmt::format("Lone UTF-16 low surrogate: \\u{:x}", utf16));
    }
    if(utf16 >= high_surrogate_range_begin && utf16 <= high_surrogate_range_end) {
        if(pos+6 >= buf.size() || buf[pos+1] != '\\' || buf[pos+2] != 'u') {
            throw std::runtime_error(fmt::format("Lone UTF-16 high surrogate: \\u{:x}", utf16));
        }
        const uint16_t utf16_low = load_utf16_hex_sequence(buf, pos+3);
        if(utf16_low < low_surrogate_range_begin || utf16_low > low_surrogate_range_end) {
            throw std::runtime_error(fmt::format("Bad UTF-16 low surrogate: \\u{:x} (after \\u{:x})", utf16_low, utf16));
        }
        const utf16_surrogate_pair surrogates {
            .high = static_cast<uint16_t>(utf16 & surrogate_byte_mask),
            .low = static_cast<uint16_t>(utf16_low & surrogate_byte_mask)
        };
        pos += 6;
        encode_utf16_codepoint_to_utf8(surrogates, res);
        return;
    }
    encode_utf16_codepoint_to_utf8(utf16, res);
}

void escape_sequence_at(std::string_view buf, size_t& pos, std::string& res) {
    if(pos >= buf.size()) {
        throw std::runtime_error("Incomplete escape sequence");
    }
    switch(buf[pos]) {
        case 'n': res.push_back('\n'); return;
        case 't': res.push_back('\t'); return;
        case 'r': res.push_back('\r'); return;
        case '"': res.push_back('"');  return;
        case '/': res.push_back('/');  return;
        case 'u': utf16_escape_sequance_at(buf, pos, res); return;
        default: throw std::runtime_error("Bad escape sequence");
    }
}

template <typename T>
concept string_mapper = std::invocable<T, std::string_view> && std::invocable<T, std::string>;

template <string_mapper M>
std::invoke_result_t<M, std::string_view> parse_json_string_impl(std::string_view buf, size_t pos, M mapper) {
    if(pos >= buf.size()) {
        throw std::runtime_error("JSON string parsing starts outside the provided buffer");
    }
    if(buf[pos] != '"') {
        throw std::runtime_error("JSON string does not start with the appropriate delimiter");
    }
    pos++;
    size_t begin = pos;
    std::string res;
    while(pos < buf.size()) {
        // TODO: this is maybe a bit rough, we could check UTF-8 sequences but that would be a pain...
        if(!std::isprint(static_cast<unsigned char>(buf[pos])) && (buf[pos] & 0b10000000) == 0) {
            throw std::runtime_error(fmt::format("Bad character in JSON string: \\x{:x}", buf[pos]));
        }
        switch(buf[pos]) {
            case '\\': {
                if(pos > begin) {
                    res.append(buf.substr(begin, pos - begin));
                }
                pos++;
                ::escape_sequence_at(buf, pos, res);
                begin = pos+1;
                break;
            }
            case '"' : {
                const std::string_view last_piece = buf.substr(begin, pos - begin);
                if(res.empty()) {
                    return mapper(last_piece);
                }
                if(!last_piece.empty()) {
                    res.append(last_piece);
                }
                return mapper(std::move(res));
            }
        }
        pos++;
    }
    throw std::runtime_error("JSON string ends too early");
}

} // anonymous namespace


namespace npp {

std::string parse_json_string(std::string_view buf, size_t pos) {
    struct map_to_string {
        std::string operator()(std::string s) { return s; }
        std::string operator()(std::string_view sv) { return std::string(sv); }
    };
    return ::parse_json_string_impl(buf, pos, map_to_string{});
}

npp::lazy_string parse_json_string_lazy(std::string_view buf, size_t pos) {
    struct map_to_lazy_string {
        npp::lazy_string operator()(std::string s) { return npp::lazy_string(std::move(s)); }
        npp::lazy_string operator()(std::string_view sv) { return sv; }
    };
    return ::parse_json_string_impl(buf, pos, map_to_lazy_string{});
}

} // namespace npp
