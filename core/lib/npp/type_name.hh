
#pragma once

#include <string_view>
#include <type_traits>


namespace npp {

template <typename T>
constexpr auto type_name() {
// #if __cplusplus >= 202002L
//     std::source_location::current().function_name();
// #else
#ifdef __clang__
    std::string_view name(__PRETTY_FUNCTION__);
    constexpr std::string_view prefix("auto npp::type_name() [T = ");
    constexpr std::string_view suffix("]");
#elif defined(__GNUC__)
    std::string_view name(__PRETTY_FUNCTION__);
    constexpr std::string_view prefix("constexpr auto npp::type_name() [with T = ");
    constexpr std::string_view suffix("]");
#elif defined(_MSC_VER)
    std::string_view name(__FUNCSIG__);
    constexpr std::string_view prefix("auto __cdecl npp::type_name<");
    constexpr std::string_view suffix(">(void)");
#else
#error "Unsupported compiler"
#endif
    name.remove_prefix(prefix.size());
    name.remove_suffix(suffix.size());
    return name;
}


template <typename E, E Val>
requires(std::is_enum_v<E>)
constexpr auto enum_name() {
    constexpr std::string_view enum_type_name = type_name<E>();
#ifdef __clang__
    std::string_view name(__PRETTY_FUNCTION__);
    constexpr std::string_view prefix("auto npp::enum_name() [E = ");
    constexpr std::string_view infix("; E Val = ");
    constexpr std::string_view suffix("]");
#elif defined(__GNUC__)
    std::string_view name(__PRETTY_FUNCTION__);
    constexpr std::string_view prefix("constexpr auto npp::enum_name() [with E = ");
    constexpr std::string_view infix("; E Val = ");
    constexpr std::string_view suffix("]");
#elif defined(_MSC_VER)
    std::string_view name(__FUNCSIG__);
    constexpr std::string_view prefix("auto __cdecl npp::enum_name<");
    constexpr std::string_view infix(", ");
    constexpr std::string_view suffix(">(void)");
#else
#error "Unsupported compiler"
#endif
    name.remove_prefix(prefix.size() + enum_type_name.size() + infix.size() + enum_type_name.size() + 2 /* "::" */);
    name.remove_suffix(suffix.size());
    return name;
}

} // namespace npp
