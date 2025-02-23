
#pragma once

#include <tuple>

#include "npp/string/lazy.hh"


namespace npp {

std::string parse_json_string(std::string_view buf, size_t pos = 0);

npp::lazy_string parse_json_string_lazy(std::string_view buf, size_t pos = 0);

} // namespace npp
