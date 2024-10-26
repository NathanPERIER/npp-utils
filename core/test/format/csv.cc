
#include <catch2/catch_test_macros.hpp>

#include <npp/format/csv.hh>


TEST_CASE("Simple CSV field") {
    CHECK(fmt::format("{}", npp::csv_field{ "hello" }) == "\"hello\"");
}

TEST_CASE("Quotes in CSV field") {
    CHECK(fmt::format("{}", npp::csv_field{ "aa\"bb" }) == "\"aa\"\"bb\"");
    CHECK(fmt::format("{}", npp::csv_field{ "\"aabb" }) == "\"\"\"aabb\"");
    CHECK(fmt::format("{}", npp::csv_field{ "aabb\"" }) == "\"aabb\"\"\"");
    CHECK(fmt::format("{}", npp::csv_field{ "\"aabb\"" }) == "\"\"\"aabb\"\"\"");
}

TEST_CASE("Consecutive quotes in CSV field") {
    CHECK(fmt::format("{}", npp::csv_field{ "aa\"\"bb" }) == "\"aa\"\"\"\"bb\"");
}

