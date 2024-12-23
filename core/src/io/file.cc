#include "npp/io/file.hh"

#include <fstream>


namespace npp {

void write_to_file(const std::string& filename, const char* data, const size_t size) {
	std::ofstream out(filename, std::ios::out | std::ios::binary);
	out.write(data, size);
	out.close();
}

void write_to_file(const std::string& filename, const uint8_t* data, const size_t size) {
	write_to_file(filename, reinterpret_cast<const char*>(data), size);
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
