
#include <catch2/catch_test_macros.hpp>

#include <npp/conv/integer.hh>

TEST_CASE("Unsigned integer conversion") {
    CHECK(true  == npp::conv<std::string_view, bool>("true"));
    CHECK(false == npp::conv<std::string_view, bool>("false"));
    CHECK(65536 == npp::conv<std::string_view, uint64_t>("65536"));
    CHECK(4242  == npp::conv<std::string_view, uint32_t>("4242"));
    CHECK(0     == npp::conv<std::string_view, uint16_t>("0"));
    CHECK(12    == npp::conv<std::string_view, uint8_t>("12"));
}

TEST_CASE("Bad unsigned integer conversion") {
    CHECK_THROWS(npp::conv<std::string_view, bool>("0"));
    CHECK_THROWS(npp::conv<std::string_view, uint64_t>("bonjour"));
    CHECK_THROWS(npp::conv<std::string_view, uint32_t>("00a"));
    CHECK_THROWS(npp::conv<std::string_view, uint16_t>("-1"));
    CHECK_THROWS(npp::conv<std::string_view, uint8_t>("0.2"));
}

TEST_CASE("Signed integer conversion") {
    CHECK(    0 == npp::conv<std::string_view, int64_t>("0"));
    CHECK(-1664 == npp::conv<std::string_view, int32_t>("-1664"));
    CHECK(  -12 == npp::conv<std::string_view, int16_t>("-12"));
    CHECK(   65 == npp::conv<std::string_view, int8_t>("65"));
}

TEST_CASE("Bad signed integer conversion") {
    CHECK_THROWS(npp::conv<std::string_view, int64_t>("oui"));
    CHECK_THROWS(npp::conv<std::string_view, int32_t>("-8°C"));
    CHECK_THROWS(npp::conv<std::string_view, int16_t>("3.14"));
    CHECK_THROWS(npp::conv<std::string_view, int8_t>("--1"));
}
