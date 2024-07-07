
#include <vector>

#include <catch2/catch_test_macros.hpp>

#include <npp/string/split.hh>



TEST_CASE("Split without separator in string") {
    CHECK(npp::split("hello", ",").to<std::vector<std::string_view>>() == std::vector<std::string_view>{ "hello" });
}

TEST_CASE("Split string one separator") {
    CHECK(npp::split("Oh, hi", ", ").to<std::vector<std::string_view>>() == std::vector<std::string_view>{ "Oh", "hi" });
}

TEST_CASE("Split string containing several separators") {
    CHECK(npp::split("straw,berry,cake", ",").to<std::vector<std::string_view>>() == std::vector<std::string_view>{ "straw", "berry", "cake" });
}

TEST_CASE("Split string containing consecutive separators") {
    CHECK(npp::split("he||o", "|").to<std::vector<std::string_view>>() == std::vector<std::string_view>{ "he", "", "o" });
}

TEST_CASE("Split string containing leading and trailing separators") {
    CHECK(npp::split("##bonjour##", "##").to<std::vector<std::string_view>>() == std::vector<std::string_view>{ "", "bonjour", "" });
}

TEST_CASE("Split empty string") {
    CHECK(npp::split("", ",").to<std::vector<std::string_view>>() == std::vector<std::string_view>{ "" });
}
