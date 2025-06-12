
#pragma once

#include <cstdint>
#include <span>
#include <string>
#include <vector>


namespace npp {

void write_to_file(const std::string& filepath, std::string_view text);
void write_to_file(const std::string& filepath, std::span<const uint8_t> binary);

std::string read_file_text(const std::string& filename);
std::vector<uint8_t> read_file_binary(const std::string& filename);

} // namespace npp
