

#include <catch2/catch_test_macros.hpp>

#include <npp/collections/pre_hashed.hh>


// Warning: due to compilation issues, this is currently not tested


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
