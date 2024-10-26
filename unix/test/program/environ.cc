
#include <catch2/catch_test_macros.hpp>

#include <unistd.h>

#include <npp/program/environ.hh>


const std::string test_variable_1 = "NPP_TEST_ENV_VAR1";
const std::string test_variable_2 = "NPP_TEST_ENV_VAR2";


TEST_CASE("Get environment variable") {
    REQUIRE(setenv(test_variable_1.c_str(), "hello", 1) == 0);
    REQUIRE(unsetenv(test_variable_2.c_str()) == 0);
    npp::program_env env = npp::program_env::from_environ();
    CHECK("hello" == env.get(test_variable_1));
    CHECK(!env.get(test_variable_2).has_value());
}

TEST_CASE("Set environment variable") {
    REQUIRE(unsetenv(test_variable_1.c_str()) == 0);
    npp::program_env env = npp::program_env::from_environ();
    CHECK(!env.get(test_variable_1).has_value());
    env.set(test_variable_1, "bonjour");
    CHECK("bonjour" == env.get(test_variable_1));
    env.set(test_variable_1, "bonsoir");
    CHECK("bonsoir" == env.get(test_variable_1));
}

TEST_CASE("Invalid environment variable") {
    npp::program_env env;
    CHECK_THROWS(env.set("A=B", "C"));
}

TEST_CASE("Unset environment variable") {
    REQUIRE(setenv(test_variable_1.c_str(), "hola", 1) == 0);
    REQUIRE(unsetenv(test_variable_2.c_str()) == 0);
    npp::program_env env = npp::program_env::from_environ();
    CHECK("hola" == env.get(test_variable_1));
    CHECK(!env.get(test_variable_2).has_value());
    env.unset(test_variable_1);
    CHECK(!env.get(test_variable_1).has_value());
    env.unset(test_variable_2);
    CHECK(!env.get(test_variable_2).has_value());
}

TEST_CASE("Init from another environment's output") {
    npp::program_env env;
    env.set(test_variable_1, "UNO");
    env.set(test_variable_2, "DOS");
    CHECK(2 == env.size());
    npp::program_env env2(env.envp());
    CHECK(env.size() == env2.size());
    CHECK(env.get(test_variable_1) == env2.get(test_variable_1));
    CHECK(env.get(test_variable_2) == env2.get(test_variable_2));
}
