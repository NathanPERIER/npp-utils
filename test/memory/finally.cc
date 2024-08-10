
#include <catch2/catch_test_macros.hpp>

#include <npp/memory/finally.hh>



TEST_CASE("Method is called at the end of the scope") {
    bool executed = false;
    [holder = npp::finally([&executed]() { executed = true; })]() {
        // Do something
    }();
    CHECK(executed);
}
