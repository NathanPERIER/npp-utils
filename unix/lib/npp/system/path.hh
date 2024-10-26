
#pragma once

#include <filesystem>
#include <optional>
#include <string>
#include <vector>


namespace npp {

std::vector<std::filesystem::path> read_path();

std::optional<std::filesystem::path> program_path(std::string_view program_name);

} // namespace npp