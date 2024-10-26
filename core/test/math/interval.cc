
#include <limits>

#include <catch2/catch_test_macros.hpp>

#include <npp/math/interval.hh>



TEST_CASE("Integer positive finite interval") {
    npp::interval i(32, 64);

    CHECK(i.is_finite());
    CHECK(i.length() == 33);
    for(int j = 32; j <= 64; j++) {
        CHECK(i.contains(j));
    }
    CHECK(!i.contains(31));
    CHECK(!i.contains(65));

    CHECK("[32, 64]" == fmt::format("{}", i));

    CHECK(i == npp::interval(std::pair(32,64)));
    CHECK(i != npp::interval(std::pair(32,std::nullopt)));
}

TEST_CASE("Integer negative and positive finite interval") {
    npp::interval i(-16, 64);

    CHECK(i.is_finite());
    CHECK(i.length() == 81);
    CHECK(!i.contains(-17));
    CHECK(!i.contains(65));
    for(int j = -16; j <= 64; j++) {
        CHECK(i.contains(j));
    }

    CHECK("[-16, 64]" == fmt::format("{}", i));
}

TEST_CASE("Integer negatively infinite interval") {
    npp::interval i(std::nullopt, 64);

    CHECK(!i.is_finite());
    CHECK(i.length() == std::nullopt);
    CHECK(i.contains(std::numeric_limits<int>::min()));
    CHECK(i.contains(-128));
    CHECK(i.contains(0));
    CHECK(i.contains(64));
    CHECK(!i.contains(65));

    CHECK("[-inf, 64]" == fmt::format("{}", i));
}

TEST_CASE("Integer positively infinite interval") {
    npp::interval i(-2, std::nullopt);

    CHECK(!i.is_finite());
    CHECK(i.length() == std::nullopt);
    CHECK(!i.contains(-3));
    CHECK(i.contains(-2));
    CHECK(i.contains(0));
    CHECK(i.contains(4096));
    CHECK(i.contains(std::numeric_limits<int>::max()));

    CHECK("[-2, +inf]" == fmt::format("{}", i));
}

TEST_CASE("Integer dual infinite interval") {
    npp::interval i(std::nullopt, std::nullopt);

    CHECK(!i.is_finite());
    CHECK(i.length() == std::nullopt);
    CHECK(i.contains(std::numeric_limits<int>::min()));
    CHECK(i.contains(-1024));
    CHECK(i.contains(0));
    CHECK(i.contains(1024));
    CHECK(i.contains(std::numeric_limits<int>::max()));

    CHECK("[-inf, +inf]" == fmt::format("{}", i));
}
