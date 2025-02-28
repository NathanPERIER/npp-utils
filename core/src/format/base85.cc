#include "npp/format/base85.hh"

#include <cassert>
#include <concepts>
#include <optional>
#include <stdexcept>

#include "npp/error.hh"
#include "npp/format/ifprint.hh"


namespace {

struct ascii85 {
    static constexpr char first_char = '!';
    static constexpr char last_char = first_char + 84;

    static_assert(last_char == 'u');

    static constexpr std::optional<char> four_zeroes_char = 'z';
    static constexpr std::optional<char> four_spaces_char = 'y';

    char get_char(uint8_t v85) {
        assert(v85 < 85);
        return first_char + v85;
    }

    uint8_t get_value(char c) {
        if(c < first_char || c > last_char) {
            throw std::runtime_error(fmt::format("Bad Ascii85 char: {}", npp::char_ifprint{c}));
        }
        return static_cast<uint8_t>(c - '!');
    }
};

struct z85 {
    static constexpr std::string_view chars =
        "0123456789"
        "abcdefghijklmnopqrstuvwxyz"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        ".-:+=^!/*?&<>()[]{}@%$#";

    static_assert(chars.size() == 85);

    static constexpr std::optional<char> four_zeroes_char = std::nullopt;
    static constexpr std::optional<char> four_spaces_char = std::nullopt;

    char get_char(uint8_t v85) {
        assert(v85 < 85);
        return chars[v85];
    }

    uint8_t get_value(char c) {
        if(c >= '0' && c <= '9') {
            return static_cast<uint8_t>(c - '0');
        }
        static constexpr uint8_t offset2 = static_cast<uint8_t>('9' - '0' + 1);
        if(c >= 'a' && c <= 'z') {
            return static_cast<uint8_t>(c - 'a') + offset2;
        }
        static constexpr uint8_t offset3 = static_cast<uint8_t>('z' - 'a' + 1) + offset2;
        if(c >= 'A' && c <= 'Z') {
            return static_cast<uint8_t>(c - 'A') + offset3;
        }
        static constexpr uint8_t offset4 = static_cast<uint8_t>('Z' - 'A' + 1) + offset3;
        const size_t pos = chars.find(c, offset4);
        if(pos == std::string_view::npos) {
            throw std::runtime_error(fmt::format("Bad z85 char: {}", npp::char_ifprint{c}));
        }
        return static_cast<uint8_t>(pos);
    }

};

template <typename T>
concept b85_alphabet = requires(T alphabet, char c, uint8_t o) {
    // TODO std::decay_t or something
    { T::four_zeroes_char } -> std::same_as<const std::optional<char>&>;
    { T::four_spaces_char } -> std::same_as<const std::optional<char>&>;
    { alphabet.get_char(o)  } -> std::same_as<char>;
    { alphabet.get_value(c) } -> std::same_as<uint8_t>;
};


template <size_t Count, b85_alphabet Alphabet>
requires (Count >= 1 && Count <= 4)
void encode_chars(const uint8_t* buf, std::string& out, Alphabet abc) {
    if constexpr (Count == 4) {
        if constexpr (Alphabet::four_zeroes_char.has_value()) {
            if(buf[0] == '\0' && buf[1] == '\0' && buf[2] == '\0' && buf[3] == '\0') {
                out.push_back(Alphabet::four_zeroes_char.value());
                return;
            }
        }
        if constexpr (Alphabet::four_spaces_char.has_value()) {
            if(buf[0] == ' ' && buf[1] == ' ' && buf[2] == ' ' && buf[3] == ' ') {
                out.push_back(Alphabet::four_spaces_char.value());
                return;
            }
        }
    }
    uint32_t val = 0;
    for(size_t i = 0; i < Count; i++) {
        val |= static_cast<uint32_t>(buf[i]) << (8 * (3 - i));
    }
    for(size_t i = 0; i <= Count; i++) {
        if(i == 4) {
            out.push_back(abc.get_char(val));
            continue;
        }
        uint32_t next_val = val / 85;
        out.push_back(abc.get_char(val - next_val * 85));
        val = next_val;
    }
}

template <b85_alphabet Alphabet>
std::string base85_encode_impl(std::span<const uint8_t> buf, Alphabet abc) {
    const size_t full_blocks = buf.size() / 4;
    const size_t last_block_size = buf.size() - full_blocks * 4;
    assert(last_block_size < 4);

    std::string res;
    res.reserve(full_blocks * 5 + last_block_size + 1);

    const uint8_t* in = buf.data();
    for(size_t i = 0; i < full_blocks; i++) {
        encode_chars<4>(in, res, abc);
        in += 4;
    }

    switch(last_block_size) {
        case 0: break;
        case 1: encode_chars<1>(in, res, abc); break;
        case 2: encode_chars<2>(in, res, abc); break;
        case 3: encode_chars<3>(in, res, abc); break;
        default: throw npp::unreachable();
    }
    return res;
}

} // anonymous namespace


namespace npp {

std::string ascii85_encode(std::span<const uint8_t> buf) {
    return ::base85_encode_impl<::ascii85>(buf, ::ascii85{});
}

} // namespace npp
