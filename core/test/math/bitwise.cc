
#include <catch2/catch_test_macros.hpp>

#include <npp/math/bitwise.hh>


TEST_CASE("Straightforward log2") {
    CHECK(0 == npp::log2(0b00000001));
    CHECK(1 == npp::log2(0b00000010));
    CHECK(2 == npp::log2(0b00000100));
    CHECK(3 == npp::log2(0b00001000));
    CHECK(4 == npp::log2(0b00010000));
    CHECK(5 == npp::log2(0b00100000));
    CHECK(6 == npp::log2(0b01000000));
    CHECK(7 == npp::log2(0b10000000));
}

TEST_CASE("Compute log2 with non powers of two") {
    CHECK(5 == npp::log2(0b00110010));
    CHECK(2 == npp::log2(0b00000111));
    CHECK(4 == npp::log2(0b00010001));
    CHECK(6 == npp::log2(0b01010101));
    CHECK(7 == npp::log2(0b11111111));
}

TEST_CASE("Bad log2") {
    CHECK_THROWS(npp::log2(0));
}
