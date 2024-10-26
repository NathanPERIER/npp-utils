
#include <catch2/catch_test_macros.hpp>

#include <cerrno>

#include <npp/system/errno.hh>



TEST_CASE("Exception on errno") {
    errno = EACCES;
    CHECK_THROWS(npp::error_from_errno());
    errno = 0;
}

TEST_CASE("No exception if errno is zero") {
    errno = 0;
    CHECK_NOTHROW(npp::error_from_errno());
}

