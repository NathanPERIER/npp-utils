
#pragma once

#include <cstdint>
#include <span>
#include <string>
#include <vector>

#include <fmt/format.h>


namespace npp {

std::string ascii85_encode(std::span<const uint8_t> buf);

inline std::string ascii85_encode(std::span<const char> buf) {
    return ascii85_encode(std::span<const uint8_t>(reinterpret_cast<const uint8_t*>(buf.data()), buf.size()));
}

} // namespace npp
