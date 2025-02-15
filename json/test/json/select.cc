
#include <catch2/catch_test_macros.hpp>

#include <npp/json/select.hh>


TEST_CASE("Simple JSON selection") {
    const nlohmann::json data = nlohmann::json::parse(R"__({
        "test": {
            "obj": {
                "value": 0
            }
        }
    })__");
    std::optional<uint32_t> res = std::nullopt;
    npp::json_select(data, [&res](const nlohmann::json& val) {
        CHECK(!res.has_value());
        res = val.get<uint32_t>();
    }, std::vector<npp::json_select_path_component>{ "test", "obj", "value" });
    REQUIRE(res.has_value());
    CHECK(0 == res.value());
}

TEST_CASE("JSON selection with list index") {
    const nlohmann::json data = nlohmann::json::parse(R"__({
        "test": [
            {
                "value": 1
            },
            {
                "value": 2
            }
        ]
    })__");
    std::optional<uint32_t> res = std::nullopt;
    npp::json_select(data, [&res](const nlohmann::json& val) {
        CHECK(!res.has_value());
        res = val.get<uint32_t>();
    }, std::vector<npp::json_select_path_component>{ "test", size_t(0), "value" });
    REQUIRE(res.has_value());
    CHECK(1 == res.value());
}

TEST_CASE("Wildcard selection on object") {
    const nlohmann::ordered_json data = nlohmann::ordered_json::parse(R"__({
        "k1": {
            "value": 1
        },
        "k2": {
            "value": 2
        },
        "k3": {
            "value": 3,
            "recursive": {
                "value": 5
            }
        },
        "k4": {
            "v": 4
        }
    })__");
    std::vector<uint32_t> res;
    npp::json_select(data, [&res](const nlohmann::ordered_json& val) {
        res.push_back(val.get<uint32_t>());
    }, std::vector<npp::json_select_path_component>{ npp::wildcard{}, "value" });
    const std::vector<uint32_t> ref = {1, 2, 3};
    CHECK(ref == res);
}

TEST_CASE("Wildcard selection on list") {
    const nlohmann::ordered_json data = nlohmann::ordered_json::parse(R"__([
        {
            "value": 1
        },
        {
            "v": 4
        },
        {
            "value": 2,
            "recursive": {
                "value": 5
            }
        },
        {
            "value": 3
        }
    ])__");
    std::vector<uint32_t> res;
    npp::json_select(data, [&res](const nlohmann::ordered_json& val) {
        res.push_back(val.get<uint32_t>());
    }, std::vector<npp::json_select_path_component>{ npp::wildcard{}, "value" });
    const std::vector<uint32_t> ref = {1, 2, 3};
    CHECK(ref == res);
}

TEST_CASE("Multi-layer wildcard selection") {
    const nlohmann::ordered_json data = nlohmann::ordered_json::parse(R"__({
        "flowers": {
            "winter": [
                {
                    "name": "Anemone"
                },
                {
                    "name": "Mimosa"
                }
            ],
            "spring": [
                {
                    "name": "Peony"
                },
                {
                    "name": "Rose"
                }
            ],
            "summer": [
                {
                    "name": "Campanula"
                },
                {
                    "name": "Thistle"
                }
            ],
            "autumn": [
                {
                    "name": "Geranium"
                },
                {
                    "name": "Aster"
                }
            ]
        }
    })__");
    std::vector<std::string> res;
    npp::json_select(data, [&res](const nlohmann::ordered_json& val) {
        res.push_back(val.get<std::string>());
    }, std::vector<npp::json_select_path_component>{ "flowers", npp::wildcard{}, npp::wildcard{}, "name" });
    const std::vector<std::string> ref = {
        "Anemone",
        "Mimosa",
        "Peony",
        "Rose",
        "Campanula",
        "Thistle",
        "Geranium",
        "Aster"
    };
    CHECK(ref == res);
}
