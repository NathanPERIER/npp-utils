#include "npp/algorithm/apriori.hh"

#include <algorithm>
#include <cassert>
#include <numeric>
#include <unordered_map>

#include "npp/error.hh"


namespace {

struct set_hash {
    template <std::unsigned_integral Int>
    std::size_t operator()(const std::set<Int>& values) const {
        const std::hash<Int> internal_hasher;
        std::size_t res = 0;
        for(Int v: values) {
            res ^= internal_hasher(v) + 0x9e3779b9 + (res<<6) + (res>>2);
        }
        return res;
    }
};


/// @brief generates a list of potentially frequent itemsets of size k+1, knowing the frequent itemsets of size k
template <std::unsigned_integral Int>
std::set<std::set<Int>> generate_candidates(std::size_t k, const std::set<std::set<Int>>& k_itemsets) {
    std::unordered_map<std::set<Int>, std::size_t, ::set_hash> candidates;
    std::set<Int> itemset_union; // We put this here to avoid multiple allocations
    // For each itemset, compute the union with all the following itemsets and store in the map
    // (only if the difference between the two itemsets is exactly one element)
    for(auto it1 = k_itemsets.begin(); it1 != k_itemsets.end(); it1++) {
        assert(it1->size() == k);
        for(auto it2 = std::next(it1); it2 != k_itemsets.end(); it2++) {
            itemset_union.clear();
            // All elements in it1 that are not in it2
            std::set_difference(it1->begin(), it1->end(), it2->begin(), it2->end(), std::inserter(itemset_union, itemset_union.begin()));
            if(itemset_union.size() != 1) {
                continue;
            }
            itemset_union.insert(it2->begin(), it2->end());
            auto it = candidates.find(itemset_union);
            if(it == candidates.end()) {
                candidates.insert({std::move(itemset_union), 1});
                itemset_union = std::set<Int>();
            } else {
                it->second++;
            }
        }
    }
    // Example for k=3
    // Suppose we have the (k+1)-itemset candidate S={a, b, c, d}
    // This yields four possible subsets : {a, b, c}, {a, b, d}, {a, c, d}, {b, c, d}
    // If all these subsets are included, then each possible pair will generate S exactly once :
    //  - {a, b, c} + {a, b, d}
    //  - {a, b, c} + {a, c, d}
    //  - {a, b, c} + {b, c, d}
    //  - {a, b, d} + {a, c, d}
    //  - {a, b, d} + {b, c, d}
    //  - {a, c, d} + {b, c, d}
    // The number of pairs is N = k + k-1 + ... + 1 = (k)(k+1)/2
    // Conversely, if we find N times S in the final result, we can deduce that all the subsets are indeed included,
    // but only if we verify these two properties :
    //  - Each k-itemset must be paired exactly once with all the other itemsets in the input
    //  - Each k-itemset in the input must be unique
    // The first property is guaranteed by the way we iterate over the input k-itemsets
    // The second property is guaranteed by the construction of the algorithm and the use of maps which guarantee key unicity
    std::set<std::set<Int>> res;
    const size_t expected_size = (k * (k+1)) / 2;
    for(const auto& e: candidates) {
        if(e.second == expected_size) {
            res.insert(e.first);
        }
    }
    return res;
}


/// @brief computes all the frequent items (<=> itemsets of size 1)
template <std::unsigned_integral Int>
std::set<std::set<Int>> generate_one_itemsets(const std::vector<std::set<Int>> itemsets, std::size_t threshold) {
    std::set<std::set<Int>> res;
    std::unordered_map<Int, std::size_t> count_items;
    for(const std::set<Int>& items: itemsets) {
        for(Int item: items) {
            auto it = count_items.find(item);
            if(it == count_items.end()) {
                count_items.insert({item, 1});
            } else {
                it->second++;
            }
        }
    }
    for(const auto& e: count_items) {
        if(e.second >= threshold) {
            res.insert({ e.first });
        }
    }
    return res;
}

// TODO use vector internally to avoid the overhead of sorting in sets
template <std::unsigned_integral Int>
std::vector<std::set<Int>> apriori_impl(const std::vector<std::set<Int>>& itemsets, std::size_t threshold) {
    std::vector<std::set<Int>> res;
    std::size_t k = 1;
    std::vector<bool> useful(itemsets.size(), true);
    std::vector<bool> actually_used;
    std::set<std::set<Int>> k_itemsets = ::generate_one_itemsets(itemsets, threshold);
    while(!k_itemsets.empty()) {
        res.insert(res.end(), k_itemsets.begin(), k_itemsets.end());
        k_itemsets = ::generate_candidates(k, k_itemsets);
        k++;
        actually_used.assign(itemsets.size(), false);
        for(auto items_it = k_itemsets.begin(); items_it != k_itemsets.end();) {
            std::size_t count = 0;
            for(std::size_t i = 0; i < itemsets.size(); i++) {
                if(!useful[i]) {
                    continue;
                }
                const std::set<Int>& input_items = itemsets[i];
                if(std::includes(input_items.begin(), input_items.end(), items_it->begin(), items_it->end())) {
                    actually_used[i] = true;
                    count++;
                }
            }
            if(count >= threshold) {
                items_it++;
                continue;
            }
            auto rem_it = items_it;
            items_it++;
            k_itemsets.erase(rem_it); // This works because we are working with sets
        }
        useful = actually_used;
    }
    return res;
}


/// @brief Implementation of itemset parsing with strings, uses an internal mapping to integers to call the methods that work on integers
/// @tparam Int integer type to use, should be chosen to be as small as possible
template <std::unsigned_integral Int>
std::vector<std::set<std::string>> apriori_string_impl(const std::vector<std::set<std::string>>& itemsets, std::size_t threshold, std::vector<std::string_view> items) {
    assert(items.size() == 0 || (items.size() - 1) <= std::numeric_limits<Int>::max());
    std::unordered_map<std::string_view, Int> mapper;
    for(Int i = 0; i < items.size(); i++) {
        mapper.insert({ std::string_view(items[i]), i });
    }
    const auto map_itemset_to_integral = [&mapper](const std::set<std::string>& itemset) -> std::set<Int> {
        std::set<Int> res;
        for(const std::string& item: itemset) {
            auto it = mapper.find(std::string_view(item));
            if(it == mapper.end()) [[unlikely]] {
                throw npp::unreachable();
            }
            res.insert(it->second);
        }
        return res;
    };
    std::vector<std::set<Int>> integral_itemsets;
    for(const std::set<std::string>& itemset: itemsets) {
        integral_itemsets.push_back(map_itemset_to_integral(itemset));
    }
    integral_itemsets = apriori_impl(integral_itemsets, threshold);
    const auto map_itemset_to_string = [&items](const std::set<Int>& itemset) -> std::set<std::string> {
        std::set<std::string> res;
        for(Int item: itemset) {
            assert(item < items.size());
            res.insert(std::string(items[item]));
        }
        return res;
    };
    std::vector<std::set<std::string>> res;
    for(const std::set<Int>& itemset: integral_itemsets) {
        res.push_back(map_itemset_to_string(itemset));
    }
    return res;
}

} // anonymous namespace


namespace npp {

std::vector<std::set<uint64_t>> apriori(const std::vector<std::set<uint64_t>>& itemsets, std::size_t threshold) {
    return apriori_impl(itemsets, threshold);
}

std::vector<std::set<uint32_t>> apriori(const std::vector<std::set<uint32_t>>& itemsets, std::size_t threshold) {
    return apriori_impl(itemsets, threshold);
}

std::vector<std::set<uint16_t>> apriori(const std::vector<std::set<uint16_t>>& itemsets, std::size_t threshold) {
    return apriori_impl(itemsets, threshold);
}

std::vector<std::set<uint8_t>> apriori(const std::vector<std::set<uint8_t>>& itemsets, std::size_t threshold) {
    return apriori_impl(itemsets, threshold);
}


std::vector<std::set<std::string>> apriori(const std::vector<std::set<std::string>>& itemsets, std::size_t threshold) {
    std::set<std::string_view> items;
    for(const std::set<std::string>& itemset: itemsets) {
        for(const std::string& item: itemset) {
            items.insert(item);
        }
    }
    const std::vector<std::string_view> items_vec(items.begin(), items.end());
    if(items_vec.size() <= std::numeric_limits<uint8_t>::max() + 1) {
        return ::apriori_string_impl<uint8_t>(itemsets, threshold, items_vec);
    }
    if(items_vec.size() <= std::numeric_limits<uint16_t>::max() + 1) {
        return ::apriori_string_impl<uint16_t>(itemsets, threshold, items_vec);
    }
    if(items_vec.size() <= std::numeric_limits<uint32_t>::max() + 1) {
        return ::apriori_string_impl<uint32_t>(itemsets, threshold, items_vec);
    }
    assert((items_vec.size() - 1) <= std::numeric_limits<uint64_t>::max());
    return ::apriori_string_impl<uint64_t>(itemsets, threshold, items_vec);
}

} // namespace npp
