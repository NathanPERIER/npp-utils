
#include <catch2/catch_test_macros.hpp>

#include <npp/format/ifprint.hh>


TEST_CASE("Printable characters") {
    CHECK("'a'" == fmt::format("{}", npp::char_ifprint{ 'a' }));
    CHECK("'Z'" == fmt::format("{}", npp::char_ifprint{ 'Z' }));
    CHECK("'#'" == fmt::format("{}", npp::char_ifprint{ '#' }));
}

TEST_CASE("Non-printable characters") {
    CHECK("\\x0" == fmt::format("{}", npp::char_ifprint{ '\0' }));
    CHECK("\\xe" == fmt::format("{}", npp::char_ifprint{ '\x0e' }));
    CHECK("\\x1f" == fmt::format("{}", npp::char_ifprint{ '\x1F' }));
    CHECK("\\n" == fmt::format("{}", npp::char_ifprint{ '\n' }));
    CHECK("\\t" == fmt::format("{}", npp::char_ifprint{ '\t' }));
    CHECK("\\r" == fmt::format("{}", npp::char_ifprint{ '\r' }));
    CHECK("\\x7f" == fmt::format("{}", npp::char_ifprint{ '\x7f' }));
}
