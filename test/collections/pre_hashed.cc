

#include <catch2/catch_test_macros.hpp>

#include <npp/collections/pre_hashed.hh>


// Warning: due to compilation issues, this is currently not tested


struct cool_thing {
    std::string name;
};

struct cool_thing_hash {
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
    const npp::prehash_umap<std::string, int> map = {
        { "hello",   1 },
        { "bonjour", 2 },
        { "ola",     3 }
    };
    const npp::pre_hashed<std::string> k1 = npp::pre_hashed<std::string>::make("goodbye");
    const npp::pre_hashed<std::string> k2 = npp::pre_hashed<std::string>::make("hello");
    CHECK(map.end() == map.find(k1));
    const auto it = map.find(k2);
    REQUIRE(it != map.end());
    CHECK(k2.value() == it->first);
    CHECK(1 == it->second);
}

TEST_CASE("Pre-hashed map with custom equality operator and hash") {
    npp::prehash_umap<cool_thing, int, cool_thing_hash, cool_thing_equal> map = {
        { cool_thing { "pony"  }, 1 },
        { cool_thing { "train" }, 2 },
        { cool_thing { "ice"   }, 3 }
    };
    const npp::pre_hashed<cool_thing, cool_thing_hash> key = npp::pre_hashed<cool_thing, cool_thing_hash>::make("train");
    const auto it = map.find(key);
    REQUIRE(it != map.end());
    CHECK(key.value() == it->first);
    CHECK(2 == it->second);
}


TEST_CASE("Pre-hashed set indexed by key type") {
    const npp::prehash_uset<std::string> set = {
        "hello",
        "bonjour",
        "ola"
    };
    const npp::pre_hashed<std::string> k1 = npp::pre_hashed<std::string>::make("goodbye");
    const npp::pre_hashed<std::string> k2 = npp::pre_hashed<std::string>::make("bonjour");
    CHECK(set.end() == set.find(k1));
    const auto it = set.find(k2);
    REQUIRE(it != set.end());
    CHECK(k2.value() == *it);
}

TEST_CASE("Pre-hashed set with custom equality operator and hash") {
    const npp::prehash_uset<cool_thing, cool_thing_hash, cool_thing_equal> set = {
        cool_thing { "pony"  },
        cool_thing { "train" },
        cool_thing { "ice"   }
    };
    const npp::pre_hashed<cool_thing, cool_thing_hash> key = npp::pre_hashed<cool_thing, cool_thing_hash>::make("train");
    const auto it = set.find(key);
    REQUIRE(it != set.end());
    CHECK(key.value() == *it);
}
