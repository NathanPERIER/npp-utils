#include "npp/algorithm/apriori.hh"

#include <algorithm>
#include <cassert>
#include <unordered_map>


namespace {

struct set_hash {
    std::size_t operator()(const std::set<uint64_t>& values) const {
        const std::hash<uint64_t> internal_hasher;
        std::size_t res = 0;
        for(uint64_t v: values) {
            res ^= internal_hasher(v) + 0x9e3779b9 + (res<<6) + (res>>2);
        }
        return res;
    }
};


/// @brief generates a list of potentially frequent itemsets of size k+1, knowing the frequent itemsets of size k
std::set<std::set<uint64_t>> generate_candidates(std::size_t k, const std::set<std::set<uint64_t>>& k_itemsets) {
    std::unordered_map<std::set<uint64_t>, std::size_t, ::set_hash> candidates;
    std::set<uint64_t> itemset_union; // We put this here to avoid multiple allocations
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
                itemset_union = std::set<uint64_t>();
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
    std::set<std::set<size_t>> res;
    const size_t expected_size = (k * (k+1)) / 2;
    for(const auto& e: candidates) {
        if(e.second == expected_size) {
            res.insert(e.first);
        }
    }
    return res;
}


/// @brief computes all the frequent items (<=> itemsets of size 1)
std::set<std::set<uint64_t>> generate_one_itemsets(const std::vector<std::set<uint64_t>> itemsets, uint64_t threshold) {
    std::set<std::set<uint64_t>> res;
    std::unordered_map<uint64_t, std::size_t> count_items;
    for(const std::set<uint64_t>& items: itemsets) {
        for(uint64_t item: items) {
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

} // anonymous namespace


namespace npp {

std::vector<std::set<uint64_t>> apriori(const std::vector<std::set<uint64_t>>& itemsets, uint64_t threshold) {
    std::vector<std::set<uint64_t>> res;
    std::size_t k = 1;
    std::vector<bool> useful(itemsets.size(), true);
    std::vector<bool> actually_used;
    std::set<std::set<uint64_t>> k_itemsets = ::generate_one_itemsets(itemsets, threshold);
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
                const std::set<uint64_t>& input_items = itemsets[i];
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

} // namespace npp
