
#pragma once

#include <string>


namespace npp::detail {

struct string_hasher {
    using is_transparent = void;
    size_t operator()(const std::string& s) const { return std::hash<std::string>{}(s);      }
    size_t operator()(std::string_view s)   const { return std::hash<std::string_view>{}(s); }
};

} // namespace npp::detail
