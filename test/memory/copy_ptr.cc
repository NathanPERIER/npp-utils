
#include <catch2/catch_test_macros.hpp>

#include <npp/memory/copy_ptr.hh>



TEST_CASE("Default init to null") {
    npp::copy_ptr<std::string> ptr1;
    CHECK(ptr1 == nullptr);
}

TEST_CASE("Pointer copies have different addresses") {
    npp::copy_ptr<std::string> ptr1 = npp::make_copy_ptr<std::string>("copy");
    npp::copy_ptr<std::string> ptr2 = ptr1;

    CHECK(ptr1 != ptr2);
    CHECK((*ptr1) == (*ptr2));
}

TEST_CASE("Pointer shares have the same address") {
    npp::copy_ptr<std::string> ptr1 = npp::make_copy_ptr<std::string>("copy");
    npp::copy_ptr<std::string> ptr2 = ptr1.share();

    CHECK(ptr1 == ptr2);
    CHECK((*ptr1) == (*ptr2));
}

TEST_CASE("Moved pointer becomes null") {
    npp::copy_ptr<std::string> ptr1 = npp::make_copy_ptr<std::string>("copy");
    npp::copy_ptr<std::string> ptr2 = std::move(ptr1);

    CHECK(ptr1 != ptr2);
    CHECK(ptr1 == nullptr);
}
