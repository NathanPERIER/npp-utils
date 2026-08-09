#include "npp/io/file.hh"

#include <fstream>
#include <iterator>

#include <fmt/format.h>


namespace {

constexpr std::size_t read_size = 4096;

} // anonymous namespace


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
    std::string res;
    if(in.fail()) {
        throw std::runtime_error("Text input stream is invalid");
    }
    while(!in.eof()) {
        const std::size_t initial_size = res.size();
        // TODO use resize_and_overwrite in C++23
        res.resize(initial_size + ::read_size);
        in.read(reinterpret_cast<char*>(res.data()) + initial_size, ::read_size);
        const std::size_t read_count = static_cast<std::size_t>(in.gcount());
        if(read_count < ::read_size) {
            res.resize(initial_size + read_count);
            break;
        }
    }
    if(in.bad() || !in.eof()) {
        throw std::runtime_error("Error while reading text input stream");
    }
    return res;
}

std::vector<uint8_t> read_istream_binary(std::istream& in) {
    std::vector<uint8_t> res;
    if(in.fail()) {
        throw std::runtime_error("Binary input stream is invalid");
    }
    while(!in.eof()) {
        const std::size_t initial_size = res.size();
        res.resize(initial_size + ::read_size);
        in.read(reinterpret_cast<char*>(res.data()) + initial_size, ::read_size);
        const std::size_t read_count = static_cast<std::size_t>(in.gcount());
        if(read_count < ::read_size) {
            res.resize(initial_size + read_count);
            break;
        }
    }
    if(in.bad() || !in.eof()) {
        throw std::runtime_error("Error while reading binary input stream");
    }
    return res;
}


std::string read_file_text(const std::string& filepath) {
    std::ifstream in(filepath, std::ios::in);
    if(in.fail()) {
        throw std::runtime_error(fmt::format("Could not open file {} for reading", filepath));
    }
    return read_istream_text(in);
}

std::vector<uint8_t> read_file_binary(const std::string& filepath) {
    std::ifstream in(filepath, std::ios::in | std::ios::binary);
    if(in.fail()) {
        throw std::runtime_error(fmt::format("Could not open file {} for reading", filepath));
    }
    return read_istream_binary(in);
}

} // namespace npp
