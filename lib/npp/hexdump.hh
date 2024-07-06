
#pragma once

#include <iostream>
#include <string>
#include <string_view>
#include <concepts>


namespace npp {

inline std::string byte_to_string(uint8_t b) {
    static const char hex_chars[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
    return std::string({ hex_chars[(b & 0b11110000) >> 4], hex_chars[b & 0b00001111] });
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

} // namespace npp
