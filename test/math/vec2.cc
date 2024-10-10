
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include <npp/math/vec2.hh>



TEST_CASE("Integer vector addition") {
    const npp::vec2<npp::N> v1(1, 2);
    const npp::vec2<npp::N> v2(4, 8);
    CHECK(npp::vec2<npp::N>(4, 5) == (v1 + 3));
    CHECK(npp::vec2<npp::N>(5, 10) == (v1 + v2));
}

TEST_CASE("Integer vector multiplication") {
    const npp::vec2<npp::N> v1(3, 5);
    const npp::vec2<npp::N> v2(-5, 3);
    CHECK(npp::vec2<npp::N>(9, 15) == (v1 * 3));
    CHECK(0 == (v1 * v2));
}

TEST_CASE("Real vector addition") {
    const npp::vec2<npp::R> v1(0.6, 1.2);
    const npp::vec2<npp::R> v2(4.5, -1.0);
    CHECK_THAT((v1 + 2.2).x, Catch::Matchers::WithinAbs(2.8, 0.001));
    CHECK_THAT((v1 + 2.2).y, Catch::Matchers::WithinAbs(3.4, 0.001));
    CHECK_THAT((v1 + v2).x, Catch::Matchers::WithinAbs(5.1, 0.001));
    CHECK_THAT((v1 + v2).y, Catch::Matchers::WithinAbs(0.2, 0.001));
}

TEST_CASE("Real vector multiplication") {
    const npp::vec2<npp::R> v1(3.5, -3.6);
    const npp::vec2<npp::R> v2(7.5, 1.3);
    CHECK_THAT((v1 * 1.1).x, Catch::Matchers::WithinAbs(3.85, 0.001));
    CHECK_THAT((v1 * 1.1).y, Catch::Matchers::WithinAbs(-3.96, 0.001));
    CHECK_THAT((v1 * v2), Catch::Matchers::WithinAbs(21.57, 0.001));
}
