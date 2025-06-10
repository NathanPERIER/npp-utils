#include "npp/io/file.hh"

#include <fstream>
#include <iterator>


namespace npp {

void write_to_file(const std::string& filepath, std::string_view text) {
	std::ofstream out(filepath, std::ios::out);
	out.write(text.data(), text.size());
}

void write_to_file(const std::string& filepath, std::span<const uint8_t> binary) {
	std::ofstream out(filepath, std::ios::out | std::ios::binary);
	out.write(reinterpret_cast<const char*>(binary.data()), binary.size());
}


std::string read_istream_text(std::istream& in) {
	return std::string(
		std::istreambuf_iterator<char>(in),
		std::istreambuf_iterator<char>()
	);
}

std::vector<uint8_t> read_istream_binary(std::istream& in) {
	return std::vector<uint8_t>(
		std::istreambuf_iterator<char>(in),
		std::istreambuf_iterator<char>()
	);
}


std::string read_file_text(const std::string& filepath) {
	std::ifstream in(filepath, std::ios::in);
	return read_istream_text(in);
}

std::vector<uint8_t> read_file_binary(const std::string& filepath) {
	std::ifstream in(filepath, std::ios::in | std::ios::binary);
	return read_istream_binary(in);
}

} // namespace npp
