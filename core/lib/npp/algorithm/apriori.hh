
#pragma once

#include <cstdint>
#include <set>
#include <string>
#include <vector>


namespace npp {

std::vector<std::set<uint64_t>> apriori(const std::vector<std::set<uint64_t>>& itemsets, std::size_t threshold);
std::vector<std::set<uint32_t>> apriori(const std::vector<std::set<uint32_t>>& itemsets, std::size_t threshold);
std::vector<std::set<uint16_t>> apriori(const std::vector<std::set<uint16_t>>& itemsets, std::size_t threshold);
std::vector<std::set<uint8_t>>  apriori(const std::vector<std::set<uint8_t>>& itemsets, std::size_t threshold);

std::vector<std::set<std::string>> apriori(const std::vector<std::set<std::string>>& itemsets, std::size_t threshold);

} // namespace npp
