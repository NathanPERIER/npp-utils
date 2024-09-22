

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
    CHECK(map.end() == map.find("goodbye"));
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


TEST_CASE("Pre-hashed set indexed by key type") {
    npp::prehash_uset<std::string> set = {
        npp::pre_hashed<std::string>::make("hello"),
        npp::pre_hashed<std::string>::make("bonjour"),
        npp::pre_hashed<std::string>::make("ola")
    };
    CHECK(set.end() == set.find("goodbye"));
    const std::string key = "bonjour";
    const auto it = set.find(key);
    REQUIRE(it != set.end());
    CHECK(key == it->value());
}

TEST_CASE("Pre-hashed set indexed by hash") {
    npp::prehash_uset<std::string> set = {
        npp::pre_hashed<std::string>::make("hello"),
        npp::pre_hashed<std::string>::make("bonjour"),
        npp::pre_hashed<std::string>::make("ola")
    };
    const size_t hash = std::hash<std::string>{}("bonjour");
    const auto it = set.find(npp::hash_index{hash});
    REQUIRE(it != set.end());
    CHECK(std::string_view("bonjour") == it->value());
    CHECK(hash == it->hash());
}

TEST_CASE("Pre-hashed set with custom equality operator") {
    npp::prehash_uset<cool_thing> set = {
        npp::pre_hashed<cool_thing>::make("pony"),
        npp::pre_hashed<cool_thing>::make("train"),
        npp::pre_hashed<cool_thing>::make("ice")
    };
    const cool_thing key{"train"};
    const auto it = set.find(key);
    REQUIRE(it != set.end());
    CHECK(key == it->value());
}
