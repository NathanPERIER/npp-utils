
#include <catch2/catch_test_macros.hpp>

#include <unordered_map>

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

TEST_CASE("JSON deserialization with custom provider method") {
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


namespace {

struct dummy_string_holder {
    std::string data;
};

} // anonymous namespace

TEST_CASE("JSON deserialization with custom converter") {
    nlohmann::json data = nlohmann::json::parse(R"__({ "message": "hello" })__");
    npp::json_reader reader = npp::json_reader(data);

    dummy_string_holder value;

    reader.read("message", value, [](const nlohmann::json& data) {
        return dummy_string_holder{ npp::convert_json<std::string>(data) };
    });
    CHECK(value.data == "hello");
}


namespace {

enum class fruit {
    strawberry,
    raspberry,
    cherry,
    tomato
};

enum class rock {
    granite,
    andesite,
    amethyst
};

struct stuff {
    fruit food;
    rock cool_mineral;
    int32_t integer;
    double real_number;

    bool operator==(const stuff& o) {
        return (food         == o.food        )
            && (cool_mineral == o.cool_mineral)
            && (integer      == o.integer     )
            && (real_number  == o.real_number );
    }
};

struct dummy_data {
    std::string name;
    uint64_t id;
    stuff some_stuff;
    uint16_t val1;
    uint16_t val2;
    int64_t old_val = 12;
    bool is_cold;

    bool operator==(const dummy_data& o) {
        return (name       == o.name      )
            && (id         == o.id        )
            && (some_stuff == o.some_stuff)
            && (val1       == o.val1      )
            && (val2       == o.val2      )
            && (old_val    == o.old_val   )
            && (is_cold    == o.is_cold   );
    }
};

} // anonymous namespace

namespace npp {

template <>
struct json_converter<::fruit> {
    template <npp::json Json>
    static ::fruit convert(const Json& data) {
        static const std::unordered_map<std::string, ::fruit> fruits_repr {
            { "strawberry", ::fruit::strawberry },
            { "raspberry",  ::fruit::raspberry  },
            { "charry",     ::fruit::cherry     },
            { "tomato",     ::fruit::tomato     }
        };
        const std::string repr = json_converter<std::string>::convert(data);
        const auto it = fruits_repr.find(repr);
        if(it == fruits_repr.end()) {
            throw std::runtime_error(fmt::format("Bad fruit : {}", repr));
        }
        return it->second;
    }
};

} // namespace npp

TEST_CASE("JSON complex structure deserialization") {
    nlohmann::json data = nlohmann::json::parse(R"__({
        "name": "John",
        "id": 718549,
        "some_stuff": {
            "food": "strawberry",
            "cool_mineral": "amethyst",
            "integer": 1
        },
        "val1": 42,
        "is_cold": true
    })__");

    ::dummy_data value;

    npp::json_reader(data)
        .read("name", value.name)
        .read("id",   value.id  )
        .read_opt("val1",      value.val1,    static_cast<uint16_t>(15u)           )
        .read_opt("val2",      value.val2,    [&val = value.val1]() { return val; })
        .read_opt("old_val",   value.old_val                                       )
        .read_opt("is_cold",   value.is_cold, false                                )
        .recurse("some_stuff", value.some_stuff, [](npp::json_reader reader, ::stuff& some_stuff) {
            reader
                .read_opt("food",         some_stuff.food,         ::fruit::raspberry)
                .read_opt("cool_mineral", some_stuff.cool_mineral, [](const nlohmann::json& data) -> ::rock {
                    static const std::unordered_map<std::string, ::rock> rocks_repr {
                        { "granite",  ::rock::granite  },
                        { "andesite", ::rock::andesite },
                        { "amethyst", ::rock::amethyst }
                    };
                    const std::string repr = npp::json_converter<std::string>::convert(data);
                    const auto it = rocks_repr.find(repr);
                    if(it == rocks_repr.end()) {
                        throw std::runtime_error(fmt::format("Bad rock : {}", repr));
                    }
                    return it->second;
                })
                .read_opt("integer",     some_stuff.integer,     12  )
                .read_opt("real_number", some_stuff.real_number, 3.14);
        });

    ::dummy_data expected {
        .name = "John",
        .id   = 718549,
        .some_stuff = {
            .food         = ::fruit::strawberry,
            .cool_mineral = ::rock::amethyst,
            .integer      = 1,
            .real_number  = 3.14
        },
        .val1 = 42,
        .val2 = 42,
        .old_val = 12,
        .is_cold = true
    };

    CHECK(expected == value);
}
