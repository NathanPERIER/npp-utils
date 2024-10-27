
#include <catch2/catch_test_macros.hpp>

#include <npp/json/reader.hh>


TEST_CASE("Basic JSON deserialization") {
    nlohmann::json data = nlohmann::json::parse(R"__({ "value": 80, "badval": "hello" })__");
    npp::json_reader reader = npp::json_reader(data);

    int value = 0;
    reader.read("value", value);
    CHECK(value == 80);
    CHECK_THROWS(reader.read("sth", value));
    CHECK_THROWS(reader.read("badval", value));

    value = 0;
    reader.read_opt("sth", value);
    CHECK(value == 0);
    reader.read_opt("value", value);
    CHECK(value == 80);
    CHECK_THROWS(reader.read_opt("badval", value));

    value = 0;
    reader.read_opt("value", value, 50);
    CHECK(value == 80);
    value = 0;
    reader.read_opt("sth", value, 50);
    CHECK(value == 50);
    CHECK_THROWS(reader.read_opt("badval", value, 50));
}

TEST_CASE("JSON deserialization with provider method") {
    nlohmann::json data = nlohmann::json::parse(R"__({ "message": "hello", "count": 2 })__");
    npp::json_reader reader = npp::json_reader(data);

    std::string value;
    uint32_t number = 5;
    auto default_provider = [&number]() -> std::string {
        return fmt::format("nb={}", number);
    };

    reader.read_opt("message", value, default_provider);
    CHECK(value == "hello");
    reader.read_opt("def", value, default_provider);
    CHECK(value == "nb=5");
    CHECK_THROWS(reader.read_opt("count", value, default_provider));
    CHECK(value == "nb=5");
}
