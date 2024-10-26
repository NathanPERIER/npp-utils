
#include <catch2/catch_test_macros.hpp>

#include <cstdlib>

#include <unistd.h>

#include <npp/system/path.hh>



TEST_CASE("Find program by name") {
    // This assumes the executable `pwd` is in PATH
    // It is not a great thing to do in a unit test but we don't really have a choice...
    const std::optional<std::filesystem::path> p = npp::program_path("pwd");
    REQUIRE(p.has_value());
    CHECK(std::filesystem::exists(p.value()));
    CHECK(p->filename() == "pwd");
}

TEST_CASE("Bad program names") {
    CHECK_THROWS(npp::program_path("bin/pwd"));
    CHECK_THROWS(npp::program_path("."));
    CHECK_THROWS(npp::program_path(".."));
}

TEST_CASE("Split PATH env variable") {
    char* path_cstr = std::getenv("PATH");
    const std::string saved_path(path_cstr != nullptr ? path_cstr : "");
    path_cstr = nullptr; // invalidated by next call
    REQUIRE(setenv("PATH", "/bin:/usr/bin:/home/user/.bin:.bin", 1) == 0);
    const std::vector<std::filesystem::path> path = npp::read_path();
    CHECK(path == std::vector<std::filesystem::path>({ "/bin", "/usr/bin", "/home/user/.bin", ".bin" }));
    REQUIRE(setenv("PATH", saved_path.c_str(), 1) == 0);
}
