
#include <catch2/catch_test_macros.hpp>

#include <npp/json/reader.hh>


TEST_CASE("Basic JSON conversion") {
    nlohmann::json data = nlohmann::json::parse("80");
    int value = npp::json_converter<int>::convert(data);
    CHECK(value == 80);
}

TEST_CASE("Filesystem path JSON conversion") {
    nlohmann::json data = nlohmann::json::parse(R"_("/path//to/my/./resource")_");
    std::filesystem::path value = npp::json_converter<std::filesystem::path>::convert(data);
    CHECK(value.string() == "/path/to/my/resource");
}

TEST_CASE("Basic JSON deserialization") {
    nlohmann::json data = nlohmann::json::parse(R"__({ "value": 80 })__");
    npp::json_reader reader = npp::json_reader(data);

    int value = 0;
    reader.read("value", value);
    CHECK(value == 80);
    CHECK_THROWS(reader.read("sth", value));
}
