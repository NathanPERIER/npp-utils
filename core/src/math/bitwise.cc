#include "npp/math/bitwise.hh"

#include <cassert>
#include <stdexcept>

#include <fmt/format.h>


namespace npp {

uint8_t log2(uint8_t v) {
    if(v == 0) {
        throw std::domain_error("Cannot compute the log2 of zero");
    }
    // Let n=8 be the length of the binary number we are working with
    // de Bruijn sequence starting with exactly log2(n)=3 zeroes
    static constexpr uint8_t de_bruijn = 0b00011101;
    static constexpr uint8_t indexes[8] = { 0, 1, 6, 2, 7, 5, 4, 3 };
    // Round to the previous power of two
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v = (v >> 1) + 1;
    const uint8_t hash = static_cast<uint8_t>(v * de_bruijn) >> /* n - log2(n) = */ 5;
    assert(hash < 8);
    return indexes[hash];
}

} // namespace npp
