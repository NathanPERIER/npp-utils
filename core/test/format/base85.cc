
#include <catch2/catch_test_macros.hpp>

#include <npp/format/base85.hh>


TEST_CASE("Simple encoding all zeroes") {
    fmt::print("{}\n", npp::ascii85_encode(std::string_view("")));
    // const std::vector<uint8_t> data_buf = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    // const std::span<const uint8_t> data = data_buf;
    // CHECK(npp::base64_encode(data.subspan(0, 1), true) == "AA==");
    // CHECK(npp::base64_encode(data.subspan(0, 2), true) == "AAA=");
    // CHECK(npp::base64_encode(data.subspan(0, 3), true) == "AAAA");
    // CHECK(npp::base64_encode(data.subspan(0, 4), true) == "AAAAAA==");
    // CHECK(npp::base64_encode(data.subspan(0, 5), true) == "AAAAAAA=");
    // CHECK(npp::base64_encode(data.subspan(0, 6), true) == "AAAAAAAA");
}
