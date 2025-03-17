
#pragma once

#include <cstdint>
#include <set>
#include <vector>


namespace npp {

std::vector<std::set<uint64_t>> apriori(const std::vector<std::set<uint64_t>>& itemsets, uint64_t threshold);

} // namespace npp
