
#include <catch2/catch_test_macros.hpp>

#include <npp/format/json_string.hh>


TEST_CASE("Empty string") {
    CHECK("" == npp::parse_json_string(R"__("")__"));
}

TEST_CASE("Simple unescaped string") {
    CHECK("a bc" == npp::parse_json_string(R"__("a bc")__"));
}

TEST_CASE("Non-ASCII string") {
    CHECK("É" == npp::parse_json_string(R"__("É")__"));
}

TEST_CASE("Simple escape sequences") {
    CHECK("\n" == npp::parse_json_string(R"__("\n")__"));
    CHECK("\t" == npp::parse_json_string(R"__("\t")__"));
    CHECK("\r" == npp::parse_json_string(R"__("\r")__"));
    CHECK("\"" == npp::parse_json_string(R"__("\"")__"));
    CHECK("/"  == npp::parse_json_string(R"__("\/")__"));
}

TEST_CASE("Escape sequences mixed with normal text") {
    CHECK("a\nb\n\nc" == npp::parse_json_string(R"__("a\nb\n\nc")__"));
}

TEST_CASE("Simple UTF-16 escape sequences") {
    CHECK(std::string({ '\0' }) == npp::parse_json_string(R"__("\u0000")__"));
    CHECK("F" == npp::parse_json_string(R"__("\u0046")__"));
    CHECK("É" == npp::parse_json_string(R"__("\u00c9")__"));
    CHECK("Ʃ" == npp::parse_json_string(R"__("\u01a9")__"));
    CHECK("ẞ" == npp::parse_json_string(R"__("\u1e9e")__"));
    CHECK("ン" == npp::parse_json_string(R"__("\u30f3")__"));
}

TEST_CASE("UTF-16 surrogate escape sequences") {
    CHECK("🐝" == npp::parse_json_string(R"__("\ud83d\udc1d")__"));
}


TEST_CASE("Parsing out of buffer") {
    CHECK_THROWS(npp::parse_json_string(R"__("aaa")__", 25));
}

TEST_CASE("Not starting with quote") {
    CHECK_THROWS(npp::parse_json_string(R"__("aaa")__", 1));
}

TEST_CASE("Unfinished strings") {
    CHECK_THROWS(npp::parse_json_string(R"__(")__"));
    CHECK_THROWS(npp::parse_json_string(R"__("aaa)__"));
    CHECK_THROWS(npp::parse_json_string(R"__("aaa\")__"));
}

TEST_CASE("Illegal characters") {
    CHECK_THROWS(npp::parse_json_string("\"\n\""));
    CHECK_THROWS(npp::parse_json_string("\"\t\""));
    CHECK_THROWS(npp::parse_json_string("\"\r\""));
    CHECK_THROWS(npp::parse_json_string("\"\0\""));
    CHECK_THROWS(npp::parse_json_string("\"\16\""));
    CHECK_THROWS(npp::parse_json_string("\"\37\""));
}

TEST_CASE("Bad UTF-16 escape sequences") {
    CHECK_THROWS(npp::parse_json_string(R"__("\u)__"));
    CHECK_THROWS(npp::parse_json_string(R"__("\u1)__"));
    CHECK_THROWS(npp::parse_json_string(R"__("\u12)__"));
    CHECK_THROWS(npp::parse_json_string(R"__("\u789)__"));
    CHECK_THROWS(npp::parse_json_string(R"__("\uabcd)__"));
    CHECK_THROWS(npp::parse_json_string(R"__("\u")__"));
    CHECK_THROWS(npp::parse_json_string(R"__("\u0")__"));
    CHECK_THROWS(npp::parse_json_string(R"__("\u00")__"));
    CHECK_THROWS(npp::parse_json_string(R"__("\u000")__"));
    CHECK_THROWS(npp::parse_json_string(R"__("\uabfg")__"));
}

TEST_CASE("Bad UTF-16 surrogates") {
    CHECK_THROWS(npp::parse_json_string(R"__("\ud8aa)__"));
    CHECK_THROWS(npp::parse_json_string(R"__("\ud8aa")__"));
    CHECK_THROWS(npp::parse_json_string(R"__("\ud8aaff")__"));
    CHECK_THROWS(npp::parse_json_string(R"__("\udcaa")__"));
    CHECK_THROWS(npp::parse_json_string(R"__("\udcaa\ud8aa")__"));
    CHECK_THROWS(npp::parse_json_string(R"__("\ud8aa\u0102")__"));
}
