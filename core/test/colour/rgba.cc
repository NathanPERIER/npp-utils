
#include <catch2/catch_test_macros.hpp>

#include <npp/colour/rgba.hh>

TEST_CASE("Bad RGBA colour representation") {
    CHECK_THROWS(npp::rgba::parse(""));
    CHECK_THROWS(npp::rgba::parse("#"));
    CHECK_THROWS(npp::rgba::parse("#abced"));
    CHECK_THROWS(npp::rgba::parse("#abcedff"));
    CHECK_THROWS(npp::rgba::parse("#abcedff00"));
    CHECK_THROWS(npp::rgba::parse("#abcedg"));
}

TEST_CASE("RGBA colour parsing") {
    CHECK(npp::rgba::parse("#000000") == npp::rgba{ .r = 0x00, .g = 0x00, .b = 0x00, .a = 0xff });
    CHECK(npp::rgba::parse("ABCDEF") == npp::rgba{ .r = 0xab, .g = 0xcd, .b = 0xef, .a = 0xff });
    CHECK(npp::rgba::parse("#0ad0F9") == npp::rgba{ .r = 0x0a, .g = 0xd0, .b = 0xf9, .a = 0xff });
    CHECK(npp::rgba::parse("DEADBEA7") == npp::rgba{ .r = 0xde, .g = 0xad, .b = 0xbe, .a = 0xa7});
    CHECK(npp::rgba::parse("#CABBA6E1") == npp::rgba{ .r = 0xca, .g = 0xbb, .b = 0xa6, .a = 0xe1 });
}

TEST_CASE("RGBA colour formatting") {
    CHECK(fmt::format("{}", npp::rgba{ .r = 0x00, .g = 0x00, .b = 0x00, .a = 0x00 }) == "#00000000");
    CHECK(fmt::format("{}", npp::rgba{ .r = 0x01, .g = 0x23, .b = 0x45, .a = 0x67 }) == "#01234567");
    CHECK(fmt::format("{}", npp::rgba{ .r = 0x89, .g = 0xab, .b = 0xcd, .a = 0xef }) == "#89abcdef");
}
