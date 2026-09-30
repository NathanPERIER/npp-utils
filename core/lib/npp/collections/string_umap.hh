
#pragma once

#include <unordered_map>

#include "npp/collections/string_hasher_impl.hh"


namespace npp {

template <typename T>
using string_umap = std::unordered_map<std::string, T, detail::string_hasher, std::equal_to<>>;

} // namespace npp
