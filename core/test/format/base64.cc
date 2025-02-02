
#include <catch2/catch_test_macros.hpp>

#include <npp/format/base64.hh>


TEST_CASE("Simple encoding all zeroes") {
    const std::vector<uint8_t> data_buf = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    const std::span<const uint8_t> data = data_buf;
    CHECK(npp::base64_encode(data.subspan(0, 1), true) == "AA==");
    CHECK(npp::base64_encode(data.subspan(0, 2), true) == "AAA=");
    CHECK(npp::base64_encode(data.subspan(0, 3), true) == "AAAA");
    CHECK(npp::base64_encode(data.subspan(0, 4), true) == "AAAAAA==");
    CHECK(npp::base64_encode(data.subspan(0, 5), true) == "AAAAAAA=");
    CHECK(npp::base64_encode(data.subspan(0, 6), true) == "AAAAAAAA");
}

TEST_CASE("Simple encoding all ones") {
    const std::vector<uint8_t> data_buf = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };
    const std::span<const uint8_t> data = data_buf;
    CHECK(npp::base64_encode(data.subspan(0, 1), true) == "/w==");
    CHECK(npp::base64_encode(data.subspan(0, 2), true) == "//8=");
    CHECK(npp::base64_encode(data.subspan(0, 3), true) == "////");
    CHECK(npp::base64_encode(data.subspan(0, 4), true) == "/////w==");
    CHECK(npp::base64_encode(data.subspan(0, 5), true) == "//////8=");
    CHECK(npp::base64_encode(data.subspan(0, 6), true) == "////////");
}

TEST_CASE("Encoding with different alphabets") {
    const std::vector<uint8_t> data = { 0xfb, 0xfd };
    CHECK(npp::base64_encode(data, true) == "+/0=");
    CHECK(npp::base64url_encode(data, true) == "-_0=");
}

TEST_CASE("Encoding with and without padding") {
    const std::vector<uint8_t> data_buf = { 0xb2, 0xeb };
    const std::span<const uint8_t> data = data_buf;
    CHECK(npp::base64_encode(data.subspan(0, 1), true) == "sg==");
    CHECK(npp::base64_encode(data.subspan(0, 2), true) == "sus=");
    CHECK(npp::base64_encode(data.subspan(0, 1), false) == "sg");
    CHECK(npp::base64_encode(data.subspan(0, 2), false) == "sus");
}

TEST_CASE("Decoding with different alphabets") {
    const std::vector<uint8_t> expected = { 0xfb, 0xfd };
    CHECK(expected == npp::base64_decode("+/0="));
    CHECK(expected == npp::base64url_decode("-_0="));
}

TEST_CASE("Decoding with and without padding") {
    const std::vector<uint8_t> expected1 = { 0xad };
    const std::vector<uint8_t> expected2 = { 0x30, 0x11 };
    CHECK(expected1 == npp::base64_decode("rQ=="));
    CHECK(expected1 == npp::base64_decode("rQ"));
    CHECK(expected2 == npp::base64_decode("MBE="));
    CHECK(expected2 == npp::base64_decode("MBE"));
}

TEST_CASE("Decode with useless trailing bits") {
    const std::vector<uint8_t> expected = { 0xc1, 0xeb };
    CHECK(expected == npp::base64_decode("wes="));
    // |     0xc1      |     0xeb      |    (none)     |
    // |---------------|---------------|---------------|
    // |1 1 0 0 0 0 0 1 1 1 1 0 1 0 1 1 0 0 - - - - - -|
    // |-----------|-----------|-----------|-----------|
    // |     w     |     e     |     s     |     =     |
    CHECK(expected == npp::base64_decode("wev="));
    // |     0xc1      |     0xeb      |    (none)     |
    // |---------------|---------------|---------------|
    // |1 1 0 0 0 0 0 1 1 1 1 0 1 0 1 1 1 1 - - - - - -|
    // |-----------|-----------|-----------|-----------|
    // |     w     |     e     |     v     |     =     |
}

TEST_CASE("Bad decode") {
    CHECK_THROWS(npp::base64_decode("AAAAA"));
    CHECK_THROWS(npp::base64_decode("AAA=="));
    CHECK_THROWS(npp::base64_decode("AAA?"));
    CHECK_THROWS(npp::base64_decode("AAA-"));
    CHECK_THROWS(npp::base64_decode("AAA_"));
    CHECK_THROWS(npp::base64url_decode("AAA+"));
    CHECK_THROWS(npp::base64url_decode("AAA/"));
}
