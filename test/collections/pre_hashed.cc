

#include <catch2/catch_test_macros.hpp>

#include <npp/collections/pre_hashed.hh>


// Warning: due to compilation issues, this is currently not tested


struct cool_thing {
    std::string name;
};

template <>
struct std::hash<cool_thing> {
    size_t operator()(const cool_thing& thing) {
        return std::hash<std::string>{}(thing.name);
    }
};

struct cool_thing_equal {
    bool operator()(const cool_thing& o1, const cool_thing& o2) const {
        return o1.name == o2.name;
    }
};


TEST_CASE("Pre-hashed map indexed by key type") {
    npp::prehash_umap<std::string, int> map;
    map.insert({ npp::pre_hashed<std::string>::make("hello"),   1 });
    map.insert({ npp::pre_hashed<std::string>::make("bonjour"), 2 });
    map.insert({ npp::pre_hashed<std::string>::make("ola"),     3 });
    const std::string key = "bonjour";
    const auto it = map.find(key);
    REQUIRE(it != map.end());
    CHECK(key == it->first.value());
    CHECK(1 == it->second);
}

TEST_CASE("Pre-hashed map indexed by hash") {
    npp::prehash_umap<std::string, int> map;
    map.insert({ npp::pre_hashed<std::string>::make("hello"),   1 });
    map.insert({ npp::pre_hashed<std::string>::make("bonjour"), 2 });
    map.insert({ npp::pre_hashed<std::string>::make("ola"),     3 });
    const size_t hash = std::hash<std::string>{}("bonjour");
    const auto it = map.find(npp::hash_index{hash});
    REQUIRE(it != map.end());
    CHECK(std::string_view("bonjour") == it->first.value());
    CHECK(hash == it->first.hash());
    CHECK(1 == it->second);
}

TEST_CASE("Pre-hashed map with custom equality operator") {
    npp::prehash_umap<cool_thing, int, cool_thing_equal> map;
    map.insert({ npp::pre_hashed<cool_thing>::make("pony"),  1 });
    map.insert({ npp::pre_hashed<cool_thing>::make("train"), 2 });
    map.insert({ npp::pre_hashed<cool_thing>::make("ice"),   3 });
    const cool_thing key{"train"};
    const auto it = map.find(key);
    REQUIRE(it != map.end());
    CHECK(key == it->first.value());
    CHECK(1 == it->second);
}
