
#include <vector>

#include <catch2/catch_test_macros.hpp>

#include <npp/type_name.hh>


TEST_CASE("Basic type name") {
    CHECK(npp::type_name<int>() == "int");
}

TEST_CASE("Complex type name") {
    CHECK(npp::type_name<std::vector<int>>() == "std::vector<int>");
}
