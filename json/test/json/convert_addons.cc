
#include <catch2/catch_test_macros.hpp>

#include <npp/json/convert_std.hh>


TEST_CASE("Filesystem path JSON conversion") {
    nlohmann::json data = nlohmann::json::parse(R"_("/path//to/my/./resource")_");
    std::filesystem::path value = npp::json_converter<std::filesystem::path>::convert(data);
    CHECK(value.string() == "/path/to/my/resource");

    data = nlohmann::json::parse("12");
    CHECK_THROWS(npp::json_converter<std::filesystem::path>::convert(data));
}
