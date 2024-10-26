
#include <vector>

#include <catch2/catch_test_macros.hpp>

#include <npp/hexdump.hh>


TEST_CASE("Simple hex dump") {
    std::vector<uint8_t> data = { 0xc3, 0x89, 0x74, 0x6f, 0x69, 0x6c, 0x65 };
    std::string repr;
    npp::hexdump(data.data(), data.size(), [&repr](const std::string_view sv) {
        for(const char c: sv) {
            if(c != ' ' && c != '\n') {
                repr.push_back(c);
            }
        }
    });
    CHECK(repr == "c389746f696c65");
}
