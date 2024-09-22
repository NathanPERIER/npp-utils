
#include <catch2/catch_test_macros.hpp>

#include <npp/string/lazy.hh>



TEST_CASE("Empty lazy string") {
    const npp::lazy_string lazy;
    CHECK(lazy->empty());
}

TEST_CASE("Lazy string initialized by C-string") {
    const char* c_str = "hello";
    const npp::lazy_string lazy(c_str);
    CHECK(lazy.view() == c_str);
}

TEST_CASE("Lazy string initialized by std::string_view") {
    const std::string_view sv = "oh, hi";
    const npp::lazy_string lazy(sv);
    CHECK(lazy.view() == sv);
}

TEST_CASE("Lazy string initialized by std::string") {
    const std::string s = "domo";
    const npp::lazy_string lazy(s);
    CHECK(lazy.view() == s);
}

TEST_CASE("Copy lazy string") {
    const std::string s = "I'm here !";
    const npp::lazy_string lazy1(s);
    const npp::lazy_string lazy2 = lazy1;
    CHECK(lazy1 == lazy2);
    CHECK(lazy1.view() == lazy2.view());
    CHECK(lazy1->data() == lazy2->data());
}

TEST_CASE("Move lazy string") {
    const std::string s = "I'm here !";
    const npp::lazy_string lazy1(s);
    const std::string_view sv1 = lazy1;
    const npp::lazy_string lazy2 = std::move(lazy1);
    CHECK(sv1 == lazy2.view());
    CHECK(sv1.data() == lazy2->data());
}

TEST_CASE("Format lazy string") {
    const npp::lazy_string lazy = "Perdu";
    CHECK(lazy.view() == fmt::format("{}", lazy));
}
