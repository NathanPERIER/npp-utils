
#include <catch2/catch_test_macros.hpp>

#include <npp/format/json_string.hh>


TEST_CASE("Parse empty string") {
    CHECK("" == npp::parse_json_string(R"__("")__"));
}

TEST_CASE("Parse simple string without escape sequences") {
    CHECK("a bc" == npp::parse_json_string(R"__("a bc")__"));
}

TEST_CASE("Parse non-ASCII string") {
    CHECK("É" == npp::parse_json_string(R"__("É")__"));
}

TEST_CASE("Parse simple escape sequences") {
    CHECK("\n" == npp::parse_json_string(R"__("\n")__"));
    CHECK("\t" == npp::parse_json_string(R"__("\t")__"));
    CHECK("\r" == npp::parse_json_string(R"__("\r")__"));
    CHECK("\"" == npp::parse_json_string(R"__("\"")__"));
    CHECK("/"  == npp::parse_json_string(R"__("\/")__"));
}

TEST_CASE("Parse escape sequences mixed with normal text") {
    CHECK("a\nb\n\nc" == npp::parse_json_string(R"__("a\nb\n\nc")__"));
}

TEST_CASE("Parse simple UTF-16 escape sequences") {
    CHECK(std::string({ '\0' }) == npp::parse_json_string(R"__("\u0000")__"));
    CHECK("F" == npp::parse_json_string(R"__("\u0046")__"));
    CHECK("É" == npp::parse_json_string(R"__("\u00c9")__"));
    CHECK("Ʃ" == npp::parse_json_string(R"__("\u01a9")__"));
    CHECK("ẞ" == npp::parse_json_string(R"__("\u1e9e")__"));
    CHECK("ン" == npp::parse_json_string(R"__("\u30f3")__"));
}

TEST_CASE("Parse UTF-16 surrogate escape sequences") {
    CHECK("🐝" == npp::parse_json_string(R"__("\ud83d\udc1d")__"));
}


TEST_CASE("Parsing out of buffer") {
    CHECK_THROWS(npp::parse_json_string(R"__("aaa")__", 25));
}

TEST_CASE("Parse sequence not starting with quote") {
    CHECK_THROWS(npp::parse_json_string(R"__("aaa")__", 1));
}

TEST_CASE("Parse unfinished strings") {
    CHECK_THROWS(npp::parse_json_string(R"__(")__"));
    CHECK_THROWS(npp::parse_json_string(R"__("aaa)__"));
    CHECK_THROWS(npp::parse_json_string(R"__("aaa\")__"));
}

TEST_CASE("Parse illegal characters") {
    CHECK_THROWS(npp::parse_json_string("\"\n\""));
    CHECK_THROWS(npp::parse_json_string("\"\t\""));
    CHECK_THROWS(npp::parse_json_string("\"\r\""));
    CHECK_THROWS(npp::parse_json_string("\"\0\""));
    CHECK_THROWS(npp::parse_json_string("\"\16\""));
    CHECK_THROWS(npp::parse_json_string("\"\37\""));
}

TEST_CASE("Parse bad UTF-16 escape sequences") {
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

TEST_CASE("Parse bad UTF-16 surrogates") {
    CHECK_THROWS(npp::parse_json_string(R"__("\ud8aa)__"));
    CHECK_THROWS(npp::parse_json_string(R"__("\ud8aa")__"));
    CHECK_THROWS(npp::parse_json_string(R"__("\ud8aaff")__"));
    CHECK_THROWS(npp::parse_json_string(R"__("\udcaa")__"));
    CHECK_THROWS(npp::parse_json_string(R"__("\udcaa\ud8aa")__"));
    CHECK_THROWS(npp::parse_json_string(R"__("\ud8aa\u0102")__"));
}


TEST_CASE("Format empty string") {
    CHECK(R"__("")__" == fmt::format("{}", npp::json_string{}));
}

TEST_CASE("Format simple string") {
    CHECK(R"__("aaa")__" == fmt::format("{}", npp::json_string{"aaa"}));
}

TEST_CASE("Format only simple escape sequences") {
    CHECK(R"__("\n")__" == fmt::format("{}", npp::json_string{"\n"}));
    CHECK(R"__("\t")__" == fmt::format("{}", npp::json_string{"\t"}));
    CHECK(R"__("\r")__" == fmt::format("{}", npp::json_string{"\r"}));
    CHECK(R"__("\"")__" == fmt::format("{}", npp::json_string{"\""}));
    CHECK(R"__("\\")__" == fmt::format("{}", npp::json_string{"\\"}));
    CHECK(R"__("\n\t\r")__" == fmt::format("{}", npp::json_string{"\n\t\r"}));
    CHECK(R"__("\"\"")__" == fmt::format("{}", npp::json_string{"\"\""}));
}

TEST_CASE("Format complex UTF-8 sequences") {
    const std::string null_str({ '\0' });
    CHECK(R"__("\u0000")__" == fmt::format("{}", npp::json_string{null_str}));
    CHECK(R"__("\u0007")__" == fmt::format("{}", npp::json_string{"\x07"}));
    CHECK(R"__("\u007f")__" == fmt::format("{}", npp::json_string{"\x7f"}));
    CHECK(R"__("Æ")__" == fmt::format("{}", npp::json_string{"Æ"}));
    CHECK(R"__("λ")__" == fmt::format("{}", npp::json_string{"λ"}));
    CHECK(R"__("Щ")__" == fmt::format("{}", npp::json_string{"Щ"}));
    CHECK(R"__("ホ")__" == fmt::format("{}", npp::json_string{"ホ"}));
    CHECK(R"__("⛺")__" == fmt::format("{}", npp::json_string{"⛺"}));
}

TEST_CASE("Format mixed text") {
    CHECK(R"__("Œuf à la coque")__" == fmt::format("{}", npp::json_string{"Œuf à la coque"}));
    CHECK(R"__("{\"a\": true, \"b\": false}")__" == fmt::format("{}", npp::json_string{R"__({"a": true, "b": false})__"}));
    CHECK(R"__("アルゴリズム")__" == fmt::format("{}", npp::json_string{"アルゴリズム"}));
    CHECK(R"__("Au revoir\n")__" == fmt::format("{}", npp::json_string{"Au revoir\n"}));
}
