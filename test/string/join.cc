
#include <catch2/catch_test_macros.hpp>

#include <npp/string/join.hh>



TEST_CASE("Join empty list") {
    CHECK(npp::join<std::string>({}, ",") == "");
}

TEST_CASE("Join list of one element") {
    CHECK(npp::join({"hello"}, ",") == "hello");
}

TEST_CASE("Join list of several elements") {
    CHECK(npp::join({"hello", "my", "friend"}, " ") == "hello my friend");
}

TEST_CASE("Join list of integers") {
    CHECK(npp::join({1, 2, 3}, "+") == "1+2+3");
}

