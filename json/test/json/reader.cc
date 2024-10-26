
#include <catch2/catch_test_macros.hpp>

#include <npp/json/reader.hh>


TEST_CASE("Basic JSON deserialization") {
    nlohmann::json data = nlohmann::json::parse(R"__({ "value": 80 })__");
    npp::json_reader reader = npp::json_reader(data);

    int value = 0;
    reader.read("value", value);
    CHECK(value == 80);
}
