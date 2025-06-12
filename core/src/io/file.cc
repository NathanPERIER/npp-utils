#include "npp/io/file.hh"

#include <fstream>


namespace npp {

void write_to_file(const std::string& filepath, std::string_view text) {
	std::ofstream out(filepath, std::ios::out);
	out.write(text.data(), text.size());
}

void write_to_file(const std::string& filepath, std::span<const uint8_t> binary) {
	std::ofstream out(filepath, std::ios::out | std::ios::binary);
	out.write(reinterpret_cast<const char*>(binary.data()), binary.size());
}


std::string read_file_text(const std::string& filename) {
	std::ifstream in(filename, std::ios::in);
	in.seekg(0, std::ios_base::end);
	size_t file_size = static_cast<size_t>(in.tellg());
	std::string res(file_size, '\0');
	in.seekg(0);
	in.read(res.data(), file_size);
	in.close();
	return res;
}

std::vector<uint8_t> read_file_binary(const std::string& filename) {
	std::ifstream in(filename, std::ios::in | std::ios::binary);
	in.seekg(0, std::ios_base::end);
	size_t file_size = static_cast<size_t>(in.tellg());
	std::vector<uint8_t> res(file_size);
	in.seekg(0);
	in.read(reinterpret_cast<char*>(res.data()), file_size);
	in.close();
	return res;
}

} // namespace npp
