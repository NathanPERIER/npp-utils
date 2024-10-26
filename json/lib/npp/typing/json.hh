
#pragma once

#include <nlohmann/json.hpp>

#include "npp/typing/any_of.hh"


namespace npp {

template<typename T>
concept json = any_of<T, nlohmann::json, nlohmann::ordered_json>;

} // namespace npp
