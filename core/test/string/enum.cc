
#include <catch2/catch_test_macros.hpp>

#include <npp/error.hh>
#include <npp/string/enum.hh>


namespace {

enum class bird {
    crow,
    magpie,
    robin,
    owl,
    tern
};

} // anonymous namespace


template <>
struct npp::enum_serialiser<::bird> {
    static const std::unordered_map<::bird, std::string> mapping;
};

const std::unordered_map<::bird, std::string> npp::enum_serialiser<::bird>::mapping = {
    { ::bird::crow,   "Crow"   },
    { ::bird::magpie, "Magpie" },
    { ::bird::robin,  "Robin"  },
    { ::bird::owl,    "Owl"    },
    { ::bird::tern,   "Tern"   }
};


TEST_CASE("Simple serialisation") {
    CHECK(fmt::format("{}", ::bird::crow) == "Crow");
}

TEST_CASE("Simple deserialisation") {
    CHECK(npp::parse_enum<::bird>("Owl") == ::bird::owl);
    CHECK(!npp::parse_enum<::bird>("Peacock").has_value());
}


namespace {

enum class messy_stuff {
    stuff1,
    stuff2,
    stuff3
};

} // anonymous namespace


template <>
struct npp::enum_serialiser<::messy_stuff> {
    static const std::unordered_map<::messy_stuff, std::string> mapping;
};

const std::unordered_map<::messy_stuff, std::string> npp::enum_serialiser<::messy_stuff>::mapping = {
    { ::messy_stuff::stuff1, "Stuff1" },
    { ::messy_stuff::stuff2, "Stuff2" }
};


TEST_CASE("Missing enum member") {
    CHECK(!npp::parse_enum<::messy_stuff>("Stuff3").has_value());
    CHECK_THROWS(fmt::format("{}", ::messy_stuff::stuff3));
}
