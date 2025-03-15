
#include <catch2/catch_test_macros.hpp>

#include <npp/format/templater.hh>


namespace {

const npp::variable_repository empty_repository;

} // anonymous namespace


TEST_CASE("Empty template") {
    const npp::format_pattern pattern("");
    CHECK(!pattern.has_variables());
    CHECK("" == pattern.format(::empty_repository));
}

TEST_CASE("Template without variables") {
    const npp::format_pattern pattern("hello");
    CHECK(!pattern.has_variables());
    CHECK("hello" == pattern.format(::empty_repository));
}

TEST_CASE("Template with only escaping") {
    const npp::format_pattern pattern("{{}}");
    CHECK(!pattern.has_variables());
    CHECK("{}" == pattern.format(::empty_repository));
}

TEST_CASE("Template with only a variable") {
    npp::format_pattern pattern("{repl}");
    CHECK(pattern.has_variables());
    const std::unordered_set<std::string_view> expected_vars = { "repl" };
    CHECK(expected_vars == pattern.variables());
    CHECK_THROWS(pattern.format(::empty_repository));
    npp::variable_repository repo;
    repo.set("repl", "value");
    CHECK("value" == pattern.format(repo));
}

TEST_CASE("Template with a variable surrounded by escapes") {
    npp::format_pattern pattern("${{{var}}}");
    CHECK(pattern.has_variables());
    const std::unordered_set<std::string_view> expected_vars = { "var" };
    CHECK(expected_vars == pattern.variables());
    CHECK_THROWS(pattern.format(::empty_repository));
    npp::variable_repository repo;
    repo.set("var", "PWD");
    CHECK("${PWD}" == pattern.format(repo));
}

TEST_CASE("Template with several variables mixed with regular text") {
    npp::format_pattern pattern("There is {what} on the {some}{stuff} and {sth} on the {other}{stuff}.");
    CHECK(pattern.has_variables());
    const std::unordered_set<std::string_view> expected_vars = { "what", "some", "stuff", "sth", "other" };
    CHECK(expected_vars == pattern.variables());
    CHECK_THROWS(pattern.format(::empty_repository));
    npp::variable_repository repo;
    repo.set("what", "a cat");
    repo.set("some", "key");
    repo.set("stuff", "board");
    repo.set("sth", "equations");
    repo.set("other", "black");
    CHECK("There is a cat on the keyboard and equations on the blackboard." == pattern.format(repo));
}

TEST_CASE("Invalid templates") {
    CHECK_THROWS(npp::format_pattern("{"));
    CHECK_THROWS(npp::format_pattern("}"));
    CHECK_THROWS(npp::format_pattern("{}"));
    CHECK_THROWS(npp::format_pattern("{;p}"));
    CHECK_THROWS(npp::format_pattern("{4ad}"));
    CHECK_THROWS(npp::format_pattern("{my-var}"));
    CHECK_THROWS(npp::format_pattern("a{{{bbbb"));
    CHECK_THROWS(npp::format_pattern("hello}}}l"));
    CHECK_THROWS(npp::format_pattern("aaa{}aaa"));
}

