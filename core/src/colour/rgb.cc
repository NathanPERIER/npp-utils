#include "npp/colour/rgb.hh"

#include <stdexcept>


namespace {

uint8_t parse_char(char c) {
    if('0' <= c && c <= '9') {
        return (c - '0');
    }
    if('a' <= c && c <= 'f') {
        return (c - 'a' + 10);
    }
    if('A' <= c && c <= 'F') {
        return (c - 'A' + 10);
    }
    throw std::runtime_error("Invalid hex character in RGBA string");
}

template <size_t offset>
uint8_t parse_byte(std::string_view repr) {
    return parse_char(repr[offset*2+1])
        + (parse_char(repr[offset*2]) << 4);
}

} // anonymous namespace


namespace npp {

rgba rgba::parse(std::string_view repr) {
    rgba res;
    if(repr.starts_with('#')) {
        repr = repr.substr(1);
    }
    if(repr.size() != 6 && repr.size() != 8) {
        throw std::runtime_error("Bad RGBA string length");
    }
    res.r = ::parse_byte<0>(repr);
    res.g = ::parse_byte<1>(repr);
    res.b = ::parse_byte<2>(repr);
    if(repr.size() > 6) {
        res.a = ::parse_byte<3>(repr);
    } else {
        res.a = 0xff;
    }
    return res;
}

rgb rgb::parse(std::string_view repr) {
    if(repr.starts_with('#')) {
        repr = repr.substr(1);
    }
    if(repr.size() != 6) {
        throw std::runtime_error("Bad RGBA string length");
    }
    return {
        .r = ::parse_byte<0>(repr),
        .g = ::parse_byte<1>(repr),
        .b = ::parse_byte<2>(repr),
    };
}

} // namespace npp
