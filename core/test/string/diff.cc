
#include <catch2/catch_test_macros.hpp>

#include <npp/string/diff.hh>
#include <npp/string/join.hh>



TEST_CASE("Diff of empty strings") {
    CHECK(std::nullopt == npp::edit_path("", ""));
}

// TODO this is not that simple
// TEST_CASE("Diff against an empty string") {
// 	std::optional<std::stack<npp::edit_op>> p1 = npp::edit_path("", npp::join({1, 2, 3}, "\n"));
// 	REQUIRE(p1 != std::nullopt);
// 	CHECK(p1->top() == npp::edit_op::add); p1->pop();
// 	CHECK(p1->top() == npp::edit_op::add); p1->pop();
// 	CHECK(p1->top() == npp::edit_op::add); p1->pop();
// 	CHECK(p1->empty());

// 	std::optional<std::stack<npp::edit_op>> p2 = npp::edit_path("AAAA", "");
// 	REQUIRE(p2 != std::nullopt);
// 	CHECK(p2->top() == npp::edit_op::del); p2->pop();
// 	CHECK(p2->empty());
// }

TEST_CASE("Diff same strings") {
	const std::optional<std::stack<npp::edit_op>> p = npp::edit_path(
		npp::join({1, 2, 3}, "\n"), 
		npp::join({1, 2, 3}, "\n")
	);
	CHECK(p == std::nullopt);
}

TEST_CASE("Diff add at beginning") {
	std::optional<std::stack<npp::edit_op>> p = npp::edit_path(
		npp::join({      3, 4}, "\n"),
		npp::join({1, 2, 3, 4}, "\n")
	);
	REQUIRE(p != std::nullopt);
	CHECK(p->top() == npp::edit_op::add);  p->pop();
	CHECK(p->top() == npp::edit_op::add);  p->pop();
	CHECK(p->top() == npp::edit_op::keep); p->pop();
	CHECK(p->top() == npp::edit_op::keep); p->pop();
	CHECK(p->empty());
}

TEST_CASE("Diff remove at beginning") {
	std::optional<std::stack<npp::edit_op>> p = npp::edit_path(
		npp::join({1, 2, 3, 4}, "\n"),
		npp::join({      3, 4}, "\n")
	);
	REQUIRE(p != std::nullopt);
	CHECK(p->top() == npp::edit_op::del);  p->pop();
	CHECK(p->top() == npp::edit_op::del);  p->pop();
	CHECK(p->top() == npp::edit_op::keep); p->pop();
	CHECK(p->top() == npp::edit_op::keep); p->pop();
	CHECK(p->empty());
}


TEST_CASE("Diff add at end") {
	std::optional<std::stack<npp::edit_op>> p = npp::edit_path(
		npp::join({1, 2      }, "\n"),
		npp::join({1, 2, 3, 4}, "\n")
	);
	REQUIRE(p != std::nullopt);
	CHECK(p->top() == npp::edit_op::keep); p->pop();
	CHECK(p->top() == npp::edit_op::keep); p->pop();
	CHECK(p->top() == npp::edit_op::add);  p->pop();
	CHECK(p->top() == npp::edit_op::add);  p->pop();
	CHECK(p->empty());
}

TEST_CASE("Diff remove at and") {
	std::optional<std::stack<npp::edit_op>> p = npp::edit_path(
		npp::join({1, 2, 3, 4}, "\n"),
		npp::join({1, 2      }, "\n")
	);
	REQUIRE(p != std::nullopt);
	CHECK(p->top() == npp::edit_op::keep); p->pop();
	CHECK(p->top() == npp::edit_op::keep); p->pop();
	CHECK(p->top() == npp::edit_op::del);  p->pop();
	CHECK(p->top() == npp::edit_op::del);  p->pop();
	CHECK(p->empty());
}

TEST_CASE("Diff remove add in the middle") {
	std::optional<std::stack<npp::edit_op>> p = npp::edit_path(
		npp::join({1,    3, 4, 5}, "\n"),
		npp::join({1, 2, 3,    5}, "\n")
	);
	REQUIRE(p != std::nullopt);
	CHECK(p->top() == npp::edit_op::keep); p->pop();
	CHECK(p->top() == npp::edit_op::add);  p->pop();
	CHECK(p->top() == npp::edit_op::keep); p->pop();
	CHECK(p->top() == npp::edit_op::del);  p->pop();
	CHECK(p->top() == npp::edit_op::keep); p->pop();
	CHECK(p->empty());
}

TEST_CASE("Diff edit lines") {
	std::optional<std::stack<npp::edit_op>> p = npp::edit_path(
		npp::join({1, 6,    4, 5}, "\n"),
		npp::join({1, 2, 3, 4, 5}, "\n")
	);
	REQUIRE(p != std::nullopt);
	CHECK(p->top() == npp::edit_op::keep); p->pop();
	CHECK((p->top() == npp::edit_op::add || p->top() == npp::edit_op::del));
	if(p->top() == npp::edit_op::add) {
		p->pop();
		CHECK(p->top() == npp::edit_op::add); p->pop();
		CHECK(p->top() == npp::edit_op::del); p->pop();
	} else {
		p->pop();
		CHECK(p->top() == npp::edit_op::add); p->pop();
		CHECK(p->top() == npp::edit_op::add); p->pop();
	}
	CHECK(p->top() == npp::edit_op::keep); p->pop();
	CHECK(p->top() == npp::edit_op::keep); p->pop();
	CHECK(p->empty());
}
