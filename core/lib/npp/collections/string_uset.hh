
#pragma once

#include <unordered_set>

#include "npp/collections/string_hasher_impl.hh"


namespace npp {

using string_uset = std::unordered_set<std::string, detail::string_hasher, std::equal_to<>>;

} // namespace npp
