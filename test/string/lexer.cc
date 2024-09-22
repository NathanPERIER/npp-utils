
#include <catch2/catch_test_macros.hpp>

#include <npp/string/lexer.hh>


TEST_CASE("Bad character in input") {
    CHECK_THROWS(npp::tokenize("1 + 2 + ù + idx"));
}

TEST_CASE("End of file at the end of the stream") {
    npp::token_stream tokens = npp::tokenize("");
    REQUIRE(tokens.eof());
    CHECK(tokens.peek().get<npp::tokens::eof>().has_value());
    CHECK_THROWS(tokens.pop());
    CHECK_THROWS(tokens.drop());
    CHECK_THROWS(tokens.discard<npp::tokens::eof>());
}

TEST_CASE("Computation operators parsing") {
    npp::token_stream tokens = npp::tokenize("+ - * /");
    REQUIRE(tokens.discard<npp::tokens::plus>());
    REQUIRE(tokens.discard<npp::tokens::minus>());
    REQUIRE(tokens.discard<npp::tokens::mul>());
    REQUIRE(tokens.discard<npp::tokens::div>());
    CHECK(tokens.eof());
}

TEST_CASE("Boolean operators parsing") {
    npp::token_stream tokens = npp::tokenize("== != < > <= >=");
    REQUIRE(tokens.discard<npp::tokens::eq>());
    REQUIRE(tokens.discard<npp::tokens::ne>());
    REQUIRE(tokens.discard<npp::tokens::lt>());
    REQUIRE(tokens.discard<npp::tokens::gt>());
    REQUIRE(tokens.discard<npp::tokens::le>());
    REQUIRE(tokens.discard<npp::tokens::ge>());
    CHECK(tokens.eof());
}

TEST_CASE("Brackets and parentheses parsing") {
    npp::token_stream tokens = npp::tokenize("() [] {}");
    REQUIRE(tokens.discard<npp::tokens::lparen>());
    REQUIRE(tokens.discard<npp::tokens::rparen>());
    REQUIRE(tokens.discard<npp::tokens::lbrack>());
    REQUIRE(tokens.discard<npp::tokens::rbrack>());
    REQUIRE(tokens.discard<npp::tokens::lcurl>());
    REQUIRE(tokens.discard<npp::tokens::rcurl>());
    CHECK(tokens.eof());
}

TEST_CASE("Identifiers parsing") {
    npp::token_stream tokens = npp::tokenize("i var x0 is_enabled _idx");
    for(std::string_view var: {"i", "var", "x0", "is_enabled", "_idx"}) {
        std::optional<npp::tokens::identifier> ident = tokens.try_get<npp::tokens::identifier>();
        REQUIRE(ident.has_value());
        CHECK(var == ident.value().name);
    }
    CHECK(tokens.eof());
}

TEST_CASE("Integer constants parsing") {
    npp::token_stream tokens = npp::tokenize("1 64"); // TODO signed integers
    for(int64_t i: {1, 64}) {
        std::optional<npp::tokens::int_cst> cst = tokens.try_get<npp::tokens::int_cst>();
        REQUIRE(cst.has_value());
        CHECK(i == cst.value().value);
    }
    CHECK(tokens.eof());
}

TEST_CASE("Skip EOL") {
    npp::token_stream tokens = npp::tokenize("+ \n - ", false);
    REQUIRE(tokens.discard<npp::tokens::plus>());
    REQUIRE(tokens.discard<npp::tokens::minus>());
    CHECK(tokens.eof());
}

TEST_CASE("Parse single EOL") {
    npp::token_stream tokens = npp::tokenize("+ \n - ", true);
    REQUIRE(tokens.discard<npp::tokens::plus>());
    REQUIRE(tokens.discard<npp::tokens::eol>());
    REQUIRE(tokens.discard<npp::tokens::minus>());
    CHECK(tokens.eof());
}

TEST_CASE("Parse multiple consecutive EOL") {
    npp::token_stream tokens = npp::tokenize("+ \n\n \n -", true);
    REQUIRE(tokens.discard<npp::tokens::plus>());
    REQUIRE(tokens.discard<npp::tokens::eol>());
    REQUIRE(tokens.discard<npp::tokens::minus>());
    CHECK(tokens.eof());
}
