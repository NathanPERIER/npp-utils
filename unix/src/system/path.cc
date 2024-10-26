#include "npp/system/path.hh"

#include <cstdlib>

#include "npp/string/split.hh"


namespace {

const std::vector<std::filesystem::path> default_path = {
    "/usr/local/bin",
    "/usr/local/sbin",
    "/usr/bin",
    "/usr/sbin",
    "/bin",
    "/sbin"
};

} // anonymous namespace


namespace npp {

std::vector<std::filesystem::path> read_path() {
    const char* path_cstr = std::getenv("PATH");
    if(path_cstr == nullptr) {
        return default_path;
    }

    std::vector<std::filesystem::path> res;
    for(const std::string_view p: npp::split(path_cstr, ":")) {
        res.emplace_back(p);
    }
    return res;
}

std::optional<std::filesystem::path> program_path(std::string_view program_name) {
    if(program_name.find('/') != std::string_view::npos || program_name == "." || program_name == "..") {
        throw std::runtime_error(fmt::format("Invalid program name: {}", program_name));
    }

    const std::vector<std::filesystem::path> path = read_path();
    std::filesystem::path p;
    for(const std::filesystem::path& bin_dir: path) {
        p = bin_dir / program_name;
        std::filesystem::file_status st = std::filesystem::status(p);
        if(st.type() == std::filesystem::file_type::regular) {
            return std::filesystem::absolute(p).lexically_normal();
        }
    }

    return std::nullopt;
}

} // namespace npp

