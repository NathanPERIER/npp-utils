#include "npp/system/errno.hh"

#include <cerrno>
#include <system_error>

#include <fmt/format.h>


namespace npp {

void error_from_errno() {
    if(errno != 0) {
        throw std::system_error(errno, std::system_category(), fmt::format("Got system error {}", errno));
    }
}

} // namespace npp
