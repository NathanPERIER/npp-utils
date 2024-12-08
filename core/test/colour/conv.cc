
#include <catch2/catch_test_macros.hpp>

#include <npp/colour/conv.hh>

TEST_CASE("Basic conversions to RGB") {
    const npp::rgb red_rgb { .r = 255, .g =   0, .b =   0 };
    const npp::hsv red_hsv { .h =   0, .s = 1.0, .v = 1.0 };
    const npp::hsl red_hsl { .h =   0, .s = 1.0, .l = 0.5 };
    CHECK(red_rgb == npp::conv<npp::hsl, npp::rgb>(red_hsl));
    CHECK(red_rgb == npp::conv<npp::hsv, npp::rgb>(red_hsv));
}
