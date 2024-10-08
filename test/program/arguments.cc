
#include <catch2/catch_test_macros.hpp>

#include <npp/program/arguments.hh>


TEST_CASE("Init with variadics") {
    npp::program_args p1("./prog");
    p1.push("test");
    p1.push("--parallel");
    p1.push("2");
    npp::program_args p2("./prog", {"test", "--parallel", "2"});
    CHECK(p1 == p2);
}

TEST_CASE("Init from program args") {
    npp::program_args p1("/path/to/prog");
    p1.push("test");
    p1.push("--file");
    p1.push("/dev/null");

    const char* argv[5] = {
        "/path/to/prog",
        "test",
        "--file",
        "/dev/null",
        nullptr
    };

    npp::program_args p2(4, argv);
    CHECK(p2.executable() == "/path/to/prog");
    CHECK(p2.size() == 3);
    CHECK(p2.argc() == 4);
    CHECK(p1 == p2);
}

TEST_CASE("Pull and push from/to args") {
    npp::program_args p("prog", {"test", "-r", "-f"});
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
    npp::program_args p1("../prog", {"test", "/path/to/file", "/dest/dir"});
    npp::program_args p2(p1.argc(), p1.argv());
    CHECK(p1 == p2);
}

TEST_CASE("Args copy") {
    npp::program_args p1("prog", {"run", "prog"});
    npp::program_args p2(p1);
    CHECK(p1 == p2);
    CHECK(p1.argv() != p2.argv());
}
