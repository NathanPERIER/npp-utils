
#include <catch2/catch_test_macros.hpp>

#include <unistd.h>

#include <npp/system/environ.hh>


const std::string test_variable = "NPP_TEST_ENV_VAR";


TEST_CASE("Get environment variable") {
    npp::environ.invalidate();
    REQUIRE(setenv(test_variable.c_str(), "hello", 1) == 0);
    const auto env = npp::environ();
    const auto it = env->find(test_variable);
    REQUIRE(it != env->end());
    CHECK(it->second == "hello");
}

TEST_CASE("Environment cache invalidation") {
    npp::environ.invalidate();
    REQUIRE(unsetenv(test_variable.c_str()) == 0);
    auto env = npp::environ();
    auto it = env->find(test_variable);
    CHECK(it == env->end());
    REQUIRE(setenv(test_variable.c_str(), "bonjour", 1) == 0);
    npp::environ.invalidate();
    env = npp::environ();
    it = env->find(test_variable);
    REQUIRE(it != env->end());
    CHECK(it->second == "bonjour");
}


