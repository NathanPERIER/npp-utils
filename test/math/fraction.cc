
#include <limits>

#include <catch2/catch_test_macros.hpp>

#include <npp/math/fraction.hh>



TEST_CASE("Construct fraction with zero as denominator") {
    CHECK_THROWS(npp::fraction<uint32_t>(1, 0));
    CHECK_THROWS(npp::fraction<uint32_t>(0, 0));
}

TEST_CASE("Fraction with zero as numerator") {
    CHECK_THROWS(npp::fraction<uint32_t>(0, 0));

    npp::fraction<uint32_t> frac(0, 512);

    CHECK(frac.num() == 0);
    CHECK(frac.den() == 1);

    CHECK_THROWS(frac.inv());
}

TEST_CASE("Multiply fractions") {
    npp::fraction<uint32_t> frac1(5, 6);
    npp::fraction<uint32_t> frac2(3, 7);

    CHECK(npp::fraction<uint32_t>(15, 42) == frac1 * frac2);
    CHECK(npp::fraction<uint32_t>(5, 2)   == frac1 * 3    );
}

TEST_CASE("Divide fractions") {
    npp::fraction<uint32_t> frac1(2, 3);
    npp::fraction<uint32_t> frac2(5, 6);

    CHECK(npp::fraction<uint32_t>(4, 5) == frac1 / frac2);
    CHECK(npp::fraction<uint32_t>(1, 6) == frac2 / 5    );
    CHECK_THROWS(frac1 / npp::fraction<uint32_t>(0));
}

TEST_CASE("Add fractions") {
    npp::fraction<uint32_t> frac1(7, 6);
    npp::fraction<uint32_t> frac2(5, 9);

    CHECK(frac1 + frac2 == npp::fraction<uint32_t>(93, 54));
    CHECK(frac1 + 2     == npp::fraction<uint32_t>(19, 6));
}

TEST_CASE("Substract fractions") {
    npp::fraction<int32_t> frac1(7, 6);
    npp::fraction<int32_t> frac2(5, 9);

    CHECK(npp::fraction<int32_t>(33, 54) == frac1 - frac2);
    CHECK(npp::fraction<int32_t>(-5, 6)  == frac1 - 2    );
}

TEST_CASE("Negative fractions") {
    npp::fraction<int32_t> frac1(-7, 6);
    npp::fraction<int32_t> frac2(7, -6);
    npp::fraction<int32_t> frac3(-7, -6);

    CHECK(frac1 == frac2);
    CHECK(frac1.num() == -7);
    CHECK(frac1.den() == 6);
    CHECK(frac2.num() == -7);
    CHECK(frac2.den() == 6);

    CHECK(frac3 == frac1 * -1);
    CHECK(frac3.num() == 7);
    CHECK(frac3.den() == 6);
}
