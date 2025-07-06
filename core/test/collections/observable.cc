
#include <catch2/catch_test_macros.hpp>

#include <string>

#include <npp/collections/observable.hh>


TEST_CASE("Lambda called when value changes") {
    npp::observable<uint64_t> obs(1);
    std::size_t times_called = 0;
    const npp::subscription sub1 = obs.subscribe([&times_called](const uint64_t& val) {
        times_called++;
        CHECK(val == 2);
    });
    CHECK(sub1.is_active());
    obs.set(2);
    CHECK(*obs == 2);
    CHECK(times_called == 1);
}

TEST_CASE("Call at subscription") {
    npp::observable<uint64_t> obs(1);
    std::size_t times_called = 0;
    const npp::subscription sub1 = obs.subscribe([&times_called](const uint64_t& val) {
        times_called++;
        CHECK(val == times_called);
    }, true);
    CHECK(sub1.is_active());
    obs.set(2);
    CHECK(times_called == 2);
}

TEST_CASE("Implicit unsubscription") {
    npp::observable<uint64_t> obs(1);
    std::size_t times_called_1 = 0;
    std::size_t times_called_2 = 0;

    const npp::subscription sub1 = obs.subscribe([&times_called_1](const uint64_t& val) {
        times_called_1++;
        CHECK(val == (times_called_1+1));
    });
    CHECK(sub1.is_active());
    obs.set(2);

    {
        npp::subscription sub2 = obs.subscribe([&times_called_2](const uint64_t& val) {
            times_called_2++;
            CHECK(val == (times_called_2+2));
        });
        CHECK(sub2.is_active());
        obs.set(3);
    }

    CHECK(sub1.is_active());
    obs.set(4);

    CHECK(times_called_1 == 3);
    CHECK(times_called_2 == 1);
}

TEST_CASE("Explicit unsubscription") {
    npp::observable<uint64_t> obs(1);
    std::size_t times_called_1 = 0;
    std::size_t times_called_2 = 0;

    const npp::subscription sub1 = obs.subscribe([&times_called_1](const uint64_t& val) {
        times_called_1++;
        CHECK(val == (times_called_1+1));
    });
    CHECK(sub1.is_active());
    obs.set(2);

    npp::subscription sub2 = obs.subscribe([&times_called_2](const uint64_t& val) {
        times_called_2++;
        CHECK(val == (times_called_2+2));
    });
    CHECK(sub2.is_active());
    obs.set(3);

    sub2.unsubscribe();
    CHECK(sub1.is_active());
    CHECK(!sub2.is_active());
    obs.set(4);

    sub2.unsubscribe();
    CHECK(sub1.is_active());
    CHECK(!sub2.is_active());
    obs.set(5);

    CHECK(times_called_1 == 4);
    CHECK(times_called_2 == 1);
}

TEST_CASE("Edit with transaction") {
    npp::observable<std::string> obs = npp::observable<std::string>::make("hello");
    std::size_t times_called = 0;
    const npp::subscription sub1 = obs.subscribe([&times_called](const std::string& val) {
        times_called++;
        CHECK(val == "ifmmp");
    });
    CHECK(sub1.is_active());
    std::shared_ptr<std::string> working = obs.working_copy();
    for(std::size_t i = 0; i < working->size(); i++) {
        (*working)[i]++;
    }
    CHECK(times_called == 0);
    CHECK(*obs == "hello");
    obs.commit_changes();
    CHECK(times_called == 1);
}

TEST_CASE("Abort transaction") {
    std::string_view initial_value = "oh, hi";
    npp::observable<std::string> obs = npp::observable<std::string>::make(initial_value);
    const npp::subscription sub1 = obs.subscribe([](const std::string& val) {
        (void) val;
        FAIL_CHECK("Listener should never be called");
    });
    CHECK(sub1.is_active());
    std::shared_ptr<std::string> working = obs.working_copy();
    *working = "goodbye";
    CHECK(*obs == initial_value);
    obs.abort_changes();
    CHECK(*obs == initial_value);
    obs.commit_changes();
    CHECK(*obs == initial_value);
}

TEST_CASE("Check edit with comparable struct") {
    struct editable {
        uint32_t i = 0;

        bool operator==(const editable&) const = default;
    };
    npp::observable<editable> obs = npp::observable<editable>::make();
    CHECK(obs->i == 0);
    CHECK(!obs.has_changes());
    obs.working_copy()->i = 0;
    CHECK(!obs.has_changes());
}

TEST_CASE("Check edit with uncomparable struct") {
    struct editable {
        uint32_t i = 0;

        bool operator==(const editable&) const = delete;
    };
    npp::observable<editable> obs = npp::observable<editable>::make();
    CHECK(obs->i == 0);
    CHECK(!obs.has_changes());
    obs.working_copy()->i = 0;
    CHECK(obs.has_changes());
}

TEST_CASE("Observable without a value") {
    bool called = false;
    npp::observable<std::monostate> obs;
    CHECK(!obs.has_changes());
    npp::subscription sub = obs.subscribe([&called]() {
        called = true;
    });
    obs.prime();
    CHECK(obs.has_changes());
    CHECK(!called);
    obs.abort_changes();
    CHECK(!obs.has_changes());
    CHECK(!called);
    obs.trigger();
    CHECK(called);
    called = false;
    CHECK(!obs.has_changes());
    obs.prime();
    obs.commit_changes();
    CHECK(called);
    called = false;
    CHECK(!obs.has_changes());
}
