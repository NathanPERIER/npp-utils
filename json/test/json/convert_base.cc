
#include <catch2/catch_test_macros.hpp>

#include <npp/json/convert_base.hh>


TEST_CASE("Basic JSON conversion") {
    nlohmann::json data = nlohmann::json::parse("80");
    int value = npp::json_converter<int>::convert(data);
    CHECK(value == 80);
}
