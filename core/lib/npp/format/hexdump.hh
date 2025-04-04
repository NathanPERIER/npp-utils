
#pragma once

#include <concepts>
#include <iostream>
#include <string_view>
#include <vector>


namespace npp {

inline std::string_view byte_to_string(uint8_t b) {
    static const std::string_view hex_repr =
        "00" "01" "02" "03" "04" "05" "06" "07" "08" "09" "0a" "0b" "0c" "0d" "0e" "0f"
        "10" "11" "12" "13" "14" "15" "16" "17" "18" "19" "1a" "1b" "1c" "1d" "1e" "1f"
        "20" "21" "22" "23" "24" "25" "26" "27" "28" "29" "2a" "2b" "2c" "2d" "2e" "2f"
        "30" "31" "32" "33" "34" "35" "36" "37" "38" "39" "3a" "3b" "3c" "3d" "3e" "3f"
        "40" "41" "42" "43" "44" "45" "46" "47" "48" "49" "4a" "4b" "4c" "4d" "4e" "4f"
        "50" "51" "52" "53" "54" "55" "56" "57" "58" "59" "5a" "5b" "5c" "5d" "5e" "5f"
        "60" "61" "62" "63" "64" "65" "66" "67" "68" "69" "6a" "6b" "6c" "6d" "6e" "6f"
        "70" "71" "72" "73" "74" "75" "76" "77" "78" "79" "7a" "7b" "7c" "7d" "7e" "7f"
        "80" "81" "82" "83" "84" "85" "86" "87" "88" "89" "8a" "8b" "8c" "8d" "8e" "8f"
        "90" "91" "92" "93" "94" "95" "96" "97" "98" "99" "9a" "9b" "9c" "9d" "9e" "9f"
        "a0" "a1" "a2" "a3" "a4" "a5" "a6" "a7" "a8" "a9" "aa" "ab" "ac" "ad" "ae" "af"
        "b0" "b1" "b2" "b3" "b4" "b5" "b6" "b7" "b8" "b9" "ba" "bb" "bc" "bd" "be" "bf"
        "c0" "c1" "c2" "c3" "c4" "c5" "c6" "c7" "c8" "c9" "ca" "cb" "cc" "cd" "ce" "cf"
        "d0" "d1" "d2" "d3" "d4" "d5" "d6" "d7" "d8" "d9" "da" "db" "dc" "dd" "de" "df"
        "e0" "e1" "e2" "e3" "e4" "e5" "e6" "e7" "e8" "e9" "ea" "eb" "ec" "ed" "ee" "ef"
        "f0" "f1" "f2" "f3" "f4" "f5" "f6" "f7" "f8" "f9" "fa" "fb" "fc" "fd" "fe" "ff";
    return hex_repr.substr(b*2, 2);
}

template <std::invocable<std::string_view> Printer>
inline void hexdump(const uint8_t* buf, const uint64_t len, Printer print) {
    for(uint16_t i = 0; i < len; i += 16) {
        const uint16_t nbytes = (len - i < 16) ? len - i : 16; 
        for(uint16_t j = 0; j < nbytes; j++) {
            print(byte_to_string(buf[i + j]));
            print(" ");
            if(j % 4 == 3) { 
                print(" ");
            }
        }
        print("\n");
    }
}

inline void hexdump(const uint8_t* buf, const uint64_t len) {
    hexdump(buf, len, [](std::string_view sv) {
        std::cout << sv;
    });
}


inline uint8_t load_hex_byte(char hb, char lb) {
    static const auto convert_hex_char = [](char c) -> uint8_t {
        if('0' <= c && c <= '9') {
            return static_cast<uint8_t>(c - '0');
        }
        if('a' <= c && c <= 'f') {
            return static_cast<uint8_t>(c - 'a' + 10);
        }
        throw std::runtime_error("Bad hexadecimal character");
    };

    return ((convert_hex_char(hb) << 4) | convert_hex_char(lb));
}

inline std::vector<uint8_t> load_hex(std::string_view hex, bool with_prefix = false) {
    size_t i = 0;
    if(hex.size() >= 2 && with_prefix && hex[0] == '0' && hex[1] == 'x') {
        i = 2;
    }
    if(hex.size() % 2 == 1 || hex.size() == 0) {
        throw std::runtime_error("Bad hexadecimal string size");
    }

    std::vector<uint8_t> res;
    res.reserve((hex.size() - i) / 2);
    for(; i < hex.size(); i += 2) {
        res.push_back(load_hex_byte(hex[i], hex[i+1]));
    }

    return res;
}

} // namespace npp
