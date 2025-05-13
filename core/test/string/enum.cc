
#include <catch2/catch_test_macros.hpp>

#include <npp/error.hh>
#include <npp/string/enum.hh>


namespace {

enum class bird {
    crow,
    magpie,
    robin,
    owl,
    tern
};

} // anonymous namespace


template <>
struct fmt::formatter<bird> {
    constexpr auto parse(fmt::format_parse_context& ctx) -> fmt::format_parse_context::iterator {
        auto it = ctx.begin();
        if (it != ctx.end() && *it != '}') {
            throw fmt::format_error("invalid bird format");
        }
        return it;
    }

    auto format(bird b, fmt::format_context& ctx) const -> fmt::format_context::iterator {
        switch(b) {
            case bird::crow:   return fmt::format_to(ctx.out(), "Crow");
            case bird::magpie: return fmt::format_to(ctx.out(), "Magpie");
            case bird::robin:  return fmt::format_to(ctx.out(), "Robin");
            case bird::owl:    return fmt::format_to(ctx.out(), "Owl");
            case bird::tern:   return fmt::format_to(ctx.out(), "Tern");
        }
        throw npp::unreachable();
    }
};


TEST_CASE("Simple deserialisation") {
    const npp::enum_parser<bird> parser = {
        bird::crow,
        bird::magpie,
        bird::robin,
        bird::owl,
        bird::tern
    };
    CHECK(parser.parse("Owl") == bird::owl);
    CHECK(!parser.parse("Peacock").has_value());
}

TEST_CASE("Bad deserializer instanciation") {
    CHECK_THROWS(npp::enum_parser<bird>({
        bird::crow,
        bird::magpie,
        bird::robin,
        bird::robin,
        bird::owl,
        bird::tern
    }));
}


namespace {

enum class messy_stuff {
    stuff1,
    stuffl,
    stuff2
};

} // anonymous namespace


template <>
struct fmt::formatter<messy_stuff> {
    constexpr auto parse(fmt::format_parse_context& ctx) -> fmt::format_parse_context::iterator {
        auto it = ctx.begin();
        if (it != ctx.end() && *it != '}') {
            throw fmt::format_error("invalid messy stuff format");
        }
        return it;
    }

    auto format(messy_stuff s, fmt::format_context& ctx) const -> fmt::format_context::iterator {
        switch(s) {
            case messy_stuff::stuff1: return fmt::format_to(ctx.out(), "stuff_1");
            case messy_stuff::stuffl: return fmt::format_to(ctx.out(), "stuff_1");
            case messy_stuff::stuff2: return fmt::format_to(ctx.out(), "stuff_2");
        }
        throw npp::unreachable();
    }
};


TEST_CASE("Bad enum declaration") {
    CHECK_THROWS(npp::enum_parser<messy_stuff>({
        messy_stuff::stuff1,
        messy_stuff::stuff2,
        messy_stuff::stuffl
    }));
}
