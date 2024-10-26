
#pragma once

#include <map>
#include <string>
#include <unordered_map>


namespace npp {

struct string_hasher {
    using is_transparent = void;
    size_t operator()(const std::string& s) const { return std::hash<std::string>{}(s);      }
    size_t operator()(std::string_view s)   const { return std::hash<std::string_view>{}(s); }
};

template <typename T>
using string_map = std::map<std::string, T, std::less<>>;

template <typename T>
using string_umap = std::unordered_map<std::string, T, string_hasher, std::equal_to<>>;

} // namespace npp
