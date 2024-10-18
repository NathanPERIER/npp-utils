
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include <npp/math/complex.hh>



TEST_CASE("Complex literal") {
    using namespace npp::literals;

    CHECK(npp::complex(1, 2) == (1 + 2_i));
    CHECK(npp::complex(1, -2) == (1 - 2_i));
}

TEST_CASE("Complex addition") {
    const npp::complex c1 = npp::complex(3, 5) + npp::complex(2, -1);
    const npp::complex c2 = npp::complex(6, 8) + 2;
    CHECK_THAT(c1.re(), Catch::Matchers::WithinAbs(5.0, 0.001)); CHECK_THAT(c1.im(), Catch::Matchers::WithinAbs(4.0, 0.001));
    CHECK_THAT(c2.re(), Catch::Matchers::WithinAbs(8.0, 0.001)); CHECK_THAT(c2.im(), Catch::Matchers::WithinAbs(8.0, 0.001));
}

TEST_CASE("Complex substraction") {
    const npp::complex c1 = npp::complex(2, 9) - npp::complex(-2, 3);
    const npp::complex c2 = npp::complex(1, 3) - 2;
    CHECK_THAT(c1.re(), Catch::Matchers::WithinAbs( 4.0, 0.001)); CHECK_THAT(c1.im(), Catch::Matchers::WithinAbs(6.0, 0.001));
    CHECK_THAT(c2.re(), Catch::Matchers::WithinAbs(-1.0, 0.001)); CHECK_THAT(c2.im(), Catch::Matchers::WithinAbs(3.0, 0.001));
}

TEST_CASE("Complex multiplication") {
    const npp::complex c1 = npp::complex(3, 2) * npp::complex(8, 4);
    const npp::complex c2 = npp::complex(-1, 5) * npp::complex(15, -3);
    CHECK_THAT(c1.re(), Catch::Matchers::WithinAbs(16.0, 0.001)); CHECK_THAT(c1.im(), Catch::Matchers::WithinAbs(28.0, 0.001));
    CHECK_THAT(c2.re(), Catch::Matchers::WithinAbs( 0.0, 0.001)); CHECK_THAT(c2.im(), Catch::Matchers::WithinAbs(78.0, 0.001));
}

TEST_CASE("Complex division") {
    const npp::complex c = npp::complex(6, 5) / npp::complex(1, 3);
    CHECK_THAT(c.re(), Catch::Matchers::WithinAbs(2.1, 0.001)); CHECK_THAT(c.im(), Catch::Matchers::WithinAbs(1.3, 0.001));
}

TEST_CASE("Complex polar form") {
    CHECK_THAT(npp::complex::polar(2, M_PI/8).re(), Catch::Matchers::WithinAbs(1.847, 0.001));
    CHECK_THAT(npp::complex::polar(2, M_PI/8).im(), Catch::Matchers::WithinAbs(0.765, 0.001));
}

TEST_CASE("Complex modulus") {
    CHECK_THAT(npp::complex(2, 2).modulus(), Catch::Matchers::WithinAbs(2 * std::sqrt(2.0), 0.001));
}

TEST_CASE("Complex argument") {
    CHECK_THAT(npp::complex(2, 2).argument(), Catch::Matchers::WithinAbs(M_PI/4, 0.001));
    CHECK_THAT(npp::complex(2, 0).argument(), Catch::Matchers::WithinAbs(0, 0.001));
    CHECK_THAT(npp::complex(0, 2).argument(), Catch::Matchers::WithinAbs(M_PI/2, 0.001));
    CHECK_THAT(npp::complex(0, -2).argument(), Catch::Matchers::WithinAbs(-1 * M_PI/2, 0.001));
}

TEST_CASE("Complex to vector") {
    const npp::complex c(1, 2);
    CHECK(npp::vec2<npp::R>(1, 2) == c.vec());
}
