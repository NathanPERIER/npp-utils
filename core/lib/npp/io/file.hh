
#pragma once

#include <cstdint>
#include <string>
#include <vector>


namespace npp {

void write_to_file(const std::string& filename, const char* data, const size_t size);
void write_to_file(const std::string& filename, const uint8_t* data, const size_t size);

std::string read_file_text(const std::string& filename);
std::vector<uint8_t> read_file_binary(const std::string& filename);

} // namespace npp
