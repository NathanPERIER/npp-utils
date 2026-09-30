
#pragma once

#include <map>
#include <string>


namespace npp {

template <typename T>
using string_map = std::map<std::string, T, std::less<>>;

} // namespace npp
