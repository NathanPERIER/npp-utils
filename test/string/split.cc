
#include <vector>

#include <catch2/catch_test_macros.hpp>

#include <npp/string/split.hh>


namespace {

std::vector<std::string_view> split_to_vector(std::string_view s, const std::string& sep) {
    npp::split splitter = npp::split(s, sep);
    return std::vector<std::string_view>(splitter.begin(), splitter.end());
}

} // anonymous namespace


TEST_CASE("Split without separator in string") {
    CHECK(::split_to_vector("hello", ",") == std::vector<std::string_view>{ "hello" });
}

TEST_CASE("Split string one separator") {
    CHECK(::split_to_vector("Oh, hi", ", ") == std::vector<std::string_view>{ "Oh", "hi" });
}

TEST_CASE("Split string containing several separators") {
    CHECK(::split_to_vector("straw,berry,cake", ",") == std::vector<std::string_view>{ "straw", "berry", "cake" });
}

TEST_CASE("Split string containing consecutive separators") {
    CHECK(::split_to_vector("he||o", "|") == std::vector<std::string_view>{ "he", "", "o" });
}

TEST_CASE("Split string containing leading and trailing separators") {
    CHECK(::split_to_vector("##bonjour##", "##") == std::vector<std::string_view>{ "", "bonjour", "" });
}

TEST_CASE("Split empty string") {
    CHECK(::split_to_vector("", ",") == std::vector<std::string_view>{ "" });
}
