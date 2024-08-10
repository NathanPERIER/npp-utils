
#include <catch2/catch_test_macros.hpp>

#include <npp/program/arguments.hh>


TEST_CASE("Init with variadics") {
    npp::program_args p1;
    p1.push("test");
    p1.push("--parallel");
    p1.push("2");
    npp::program_args p2({"test", "--parallel", "2"});
    CHECK(p1 == p2);
}

TEST_CASE("Init from program args") {
    npp::program_args p1;
    p1.push("test");
    p1.push("--file");
    p1.push("/dev/null");

    char* argv[4] = {
        const_cast<char*>("/path/to/prog"),
        const_cast<char*>("test"),
        const_cast<char*>("--file"),
        const_cast<char*>("/dev/null")
    };

    npp::program_args p2(4, argv);
    CHECK(p1 == p2);

    npp::program_args p3(3, argv+1, false);
    CHECK(p1 == p3);
}

TEST_CASE("Pull and push from/to args") {
    npp::program_args p({"test", "-r", "-f"});
    CHECK(p.drop("test"));
    CHECK(p.drop("-r"));
    p.push("--recursive");
    CHECK(p.drop("-f"));
    CHECK(p.peek() == "--recursive");
    p.pop();
    CHECK_THROWS(p.drop());
    CHECK_THROWS(p.peek());
    CHECK_THROWS(p.pop());
}

TEST_CASE("Init from other arg's output") {
    npp::program_args p1({"test", "/path/to/file", "/dest/dir"});
    npp::program_args p2(p1.argc(), p1.argv(), false);
    CHECK(p1 == p2);
}
