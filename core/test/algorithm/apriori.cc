
#include <catch2/catch_test_macros.hpp>

#include <algorithm>

#include <npp/algorithm/apriori.hh>


template <typename T>
bool sort_itemsets(const std::set<T>& s1, const std::set<T>& s2) {
    if(s1.size() == s2.size()) {
        return s1 < s2;
    }
    return (s1.size() < s2.size());
}


TEST_CASE("Apriori integral test") {
    const std::vector<std::set<uint64_t>> data = {
        { 1, 2, 4 },
        { 1, 2, 5, 7 },
        { 1, 2, 5, 8, 3 },
        { 5, 3, 6 },
        { 1, 2, 6, 3 },
        { 1, 3, 6, 5 },
        { 3, 6, 2 },
        { 5, 1, 2 },
        { 1, 7, 9, 2, 5 },
        { 65536 }
    };
    const std::vector<std::set<uint64_t>> expected = {
        { 1 },
        { 2 },
        { 3 },
        { 5 },
        { 6 },
        { 1, 2 },
        { 1, 5 },
        { 2, 5 },
        { 3, 6 },
        { 1, 2, 5 }
    };
    // Get the itemsets with at least 40% of occurence
    std::vector<std::set<uint64_t>> res = npp::apriori(data, data.size() * 2 / 5);
    std::sort(res.begin(), res.end(), sort_itemsets<uint64_t>);
    CHECK(expected == res);
}


TEST_CASE("Apriori string test") {
    const std::vector<std::set<std::string>> data = {
        { "a", "b", "c" },
        { "a", "b", "d" },
        { "a", "c", "e" },
        { "a", "d" },
        { "a", "c", "f" },
        { "a", "b", "d", "e" },
        { "a", "b", "d", "f" },
        { "a", "b", "d" },
        { "g", "l" }
    };
    const std::vector<std::set<std::string>> expected = {
        { "a" },
        { "b" },
        { "c" },
        { "d" },
        { "a", "b" },
        { "a", "c" },
        { "a", "d" },
        { "b", "d" },
        { "a", "b", "d" }
    };
    std::vector<std::set<std::string>> res = npp::apriori(data, data.size() / 3);
    std::sort(res.begin(), res.end(), sort_itemsets<std::string>);
    CHECK(expected == res);
}
