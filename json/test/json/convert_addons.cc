
#include <catch2/catch_test_macros.hpp>

#include <npp/json/convert_std.hh>


TEST_CASE("Filesystem path JSON conversion") {
    nlohmann::json data = nlohmann::json::parse(R"_("/path//to/my/./resource")_");
    std::filesystem::path value = npp::convert_json<std::filesystem::path>(data);
    CHECK(value.string() == "/path/to/my/resource");

    data = nlohmann::json::parse("12");
    CHECK_THROWS(npp::convert_json<std::filesystem::path>(data));
}

TEST_CASE("Duration JSON conversion") {
    nlohmann::json data = nlohmann::json::parse("150");
    std::chrono::milliseconds value = npp::convert_json<std::chrono::milliseconds>(data);
    CHECK(value == std::chrono::milliseconds(150));

    data = nlohmann::json::parse("true");
    CHECK_THROWS(npp::convert_json<std::chrono::milliseconds>(data));
}
