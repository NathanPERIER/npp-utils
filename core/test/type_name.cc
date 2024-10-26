
#include <vector>

#include <catch2/catch_test_macros.hpp>

#include <npp/type_name.hh>

#include <fmt/format.h>

enum class test_fruits {
    strawberry,
    raspberry,
    cranberry
};


TEST_CASE("Basic type name") {
    CHECK(npp::type_name<int>() == "int");
}

TEST_CASE("Complex type name") {
    CHECK(npp::type_name<std::vector<int>>() == "std::vector<int>");
}

TEST_CASE("Enum type name") {
    CHECK(npp::type_name<test_fruits>() == "test_fruits");
}

TEST_CASE("Enum value type name") {
    CHECK(npp::enum_name<test_fruits, test_fruits::strawberry>() == "strawberry");
}
