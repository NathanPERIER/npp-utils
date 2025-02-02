#include "npp/format/base64.hh"

#include <stdexcept>


namespace {

template <char Char62, char Char63>
uint8_t decode_sextet(char sxt) {
    if(sxt >= 'A' && sxt <= 'Z') {
        return static_cast<uint8_t>(sxt - 'A');
    }
    if(sxt >= 'a' && sxt <= 'z') {
        return static_cast<uint8_t>(sxt - 'a' + 26);
    }
    if(sxt >= '0' && sxt <= '9') {
        return static_cast<uint8_t>(sxt - '0' + 52);
    }
    if(sxt == Char62) {
        return 62;
    }
    if(sxt == Char63) {
        return 63;
    }
    throw std::runtime_error(fmt::format("Bad base64 character: \\x{:x}", sxt));
}

template <char Char62, char Char63>
char encode_sextet(uint8_t sxt) {
    if(sxt < 26) {
        return 'A' + static_cast<char>(sxt);
    }
    if(sxt < 52) {
        return 'a' + static_cast<char>(sxt - 26);
    }
    if(sxt < 62) {
        return '0' + static_cast<char>(sxt - 52);
    }
    if(sxt == 62) {
        return Char62;
    }
    if(sxt == 63) {
        return Char63;
    }
    throw std::runtime_error(fmt::format("Bad base64 sextet: {:#x}", sxt));
}


/// @tparam Count the number of bytes to encode
template <size_t Count, char Char62, char Char63>
requires (Count > 0 && Count <= 3)
void encode_bytes(const uint8_t* data, char* out) {
    // |    data[0]    |    data[1]    |    data[2]    |
    // |b b b b b b b b|b b b b b b b b|b b b b b b b b|
    // |  out[0]   |  out[1]   |  out[2]   |  out[3]   |
    out[0] = encode_sextet<Char62, Char63>(data[0] >> 2);
    out[1] = encode_sextet<Char62, Char63>(((data[0] << 4) & 0b00110000) | (Count > 1 ? data[1] >> 4 : 0));
    if(Count == 1) {
        return;
    }
    out[2] = encode_sextet<Char62, Char63>(((data[1] << 2) & 0b00111100) | (Count > 2 ? data[2] >> 6 : 0));
    if(Count == 2) {
        return;
    }
    out[3] = encode_sextet<Char62, Char63>(data[2] & 0b00111111);
}

/// @tparam Count the number of characters to decode
/// @note this method is designed to enable decoding in-place
template <size_t Count, char Char62, char Char63>
requires (Count >= 2 && Count <= 4)
void decode_chars(const char* encoded, uint8_t* out) {
    // s0: 0 0 d0[0] d0[1] d0[2] d0[3] d0[4] d0[5]
    // s1: 0 0 d0[6] d0[7] d1[0] d1[1] d1[2] d1[3]
    // s2: 0 0 d1[4] d1[5] d1[6] d1[7] d2[0] d2[1]
    // s3: 0 0 d2[2] d2[3] d2[4] d2[5] d2[6] d2[7]
    const uint8_t s1 = decode_sextet<Char62, Char63>(encoded[1]);
    out[0] = (decode_sextet<Char62, Char63>(encoded[0]) << 2) | (s1 >> 4);
    if(Count == 2) {
        return;
    }
    const uint8_t s2 = decode_sextet<Char62, Char63>(encoded[2]);
    out[1] = (s1 << 4) | (s2 >> 2);
    if(Count == 3) {
        return;
    }
    out[2] = (s2 << 6) | decode_sextet<Char62, Char63>(encoded[3]);
}


template <char Char62, char Char63>
std::string base64_encode(std::span<const uint8_t> data, bool padding) {
    const size_t full_three_bytes = data.size() / 3;
    const size_t remaining = data.size() - full_three_bytes * 3;
    size_t expected_size = full_three_bytes * 4;
    if(remaining > 0) {
        expected_size += (padding ? 4 : (remaining + 1));
    }
    std::string res(expected_size, 0);
    const uint8_t* in_it = data.data();
    char* out_it = res.data();
    for(size_t i = 0; i < full_three_bytes; i++) {
        encode_bytes<3, Char62, Char63>(in_it, out_it);
        in_it += 3;
        out_it += 4;
    }
    if(remaining == 1) {
        encode_bytes<1, Char62, Char63>(in_it, out_it);
        if(padding) {
            *(out_it + 2) = '=';
            *(out_it + 3) = '=';
        }
    } else if(remaining == 2) {
        encode_bytes<2, Char62, Char63>(in_it, out_it);
        if(padding) {
            *(out_it + 3) = '=';
        }
    }
    return res;
}


template <char Char62, char Char63>
std::vector<uint8_t> base64_decode(std::string_view encoded) {
    size_t trailing_chars = encoded.size() & 0b00000011; // size % 4
    if(trailing_chars == 1) {
        throw std::runtime_error("Bad base64 encoded string length");
    }
    if(trailing_chars == 0) {
        if(encoded.ends_with("==")) {
            encoded = encoded.substr(0, encoded.length() - 2);
            trailing_chars = 2;
        } else if(encoded.ends_with('=')) {
            encoded = encoded.substr(0, encoded.length() - 1);
            trailing_chars = 3;
        }
    }
    const size_t full_three_bytes = encoded.size() / 4;
    const size_t trailing_bytes = (trailing_chars > 0) ? (trailing_chars - 1) : 0;
    std::vector<uint8_t> res(3 * full_three_bytes + trailing_bytes);
    const char* in_it = encoded.data();
    uint8_t* out_it = res.data();
    for(size_t i = 0; i < full_three_bytes; i++) {
        decode_chars<4, Char62, Char63>(in_it, out_it);
        in_it += 4;
        out_it += 3;
    }
    if(trailing_chars == 2) {
        decode_chars<2, Char62, Char63>(in_it, out_it);
    } else if(trailing_chars == 3) {
        decode_chars<3, Char62, Char63>(in_it, out_it);
    }
    return res;
}

} // anonymous namespace


namespace npp {

std::string base64_encode(std::span<const uint8_t> data, bool padding) {
    return ::base64_encode<'+', '/'>(data, padding);
}
std::string base64url_encode(std::span<const uint8_t> data, bool padding) {
    return ::base64_encode<'-', '_'>(data, padding);
}

std::vector<uint8_t> base64_decode(std::string_view data) {
    return ::base64_decode<'+', '/'>(data);
}
std::vector<uint8_t> base64url_decode(std::string_view data) {
    return ::base64_decode<'-', '_'>(data);
}

} // namespace npp
