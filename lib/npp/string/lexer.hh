
#pragma once

#include <memory>
#include <optional>
#include <queue>
#include <string>
#include <variant>

#include <fmt/format.h>

#include "npp/type_name.hh"
#include "npp/typing/variant.hh"


namespace npp::tokens {

// Identififer
struct identifier { std::string_view name; };

// Constants
struct int_cst { int64_t value; };

// Computation operators
struct plus  {};
struct minus {};
struct mul   {};
struct div   {};

// Comparison operators
struct eq {};
struct ne {};
struct gt {};
struct lt {};
struct ge {};
struct le {};

// End of line
struct eol {};

// End of file
struct eof {};

} // namespace npp::tokens


namespace npp {

using token_variant = std::variant<
    tokens::identifier,
    tokens::int_cst,
    tokens::plus,
    tokens::minus,
    tokens::mul,
    tokens::div,
    tokens::eq,
    tokens::ne,
    tokens::gt,
    tokens::lt,
    tokens::ge,
    tokens::le,
    tokens::eol,
    tokens::eof
>;

template <typename T>
concept token_option = variant_alternative<T, token_variant>;

template <token_option Token>
constexpr std::string_view token_type_name() { return npp::type_name<Token>().substr(13); }



class lexer_token {

public:
    struct position { size_t idx; size_t line; size_t chr; };

    lexer_token(token_variant tok, position begin_pos, size_t len = 1):   _token(tok), _begin(begin_pos), _end({begin_pos.idx + len, begin_pos.line, begin_pos.chr + len}) {}
    lexer_token(token_variant tok, position begin_pos, position end_pos): _token(tok), _begin(begin_pos), _end(end_pos)                                                    {}

    const position& begin_pos() const { return _begin; }
    const position& end_pos()   const { return _end;   }

    const token_variant& raw() const { return _token; }

    template <token_option Token>
    std::optional<Token> get() const {
        const Token* res = std::get_if<Token>(&_token);
        return (res == nullptr) ? std::nullopt : std::optional<Token>(*res);
    }

private:
    token_variant _token;
    position _begin;
    position _end;

};


class token_stream {

public:
    token_stream(token_stream&&)            = default;
    token_stream& operator=(token_stream&&) = default;

    token_stream(const token_stream&)            = delete;
    token_stream& operator=(const token_stream&) = delete;

    std::shared_ptr<std::string> share_buffer() const { return _buffer; }

    bool eof() const { return _tokens.size() == 1; } // last token is always eof

    /// @brief fetches a reference to the current token without removing it
    const lexer_token& peek() const { return _tokens.front(); }

    /// @brief removes the current token from the stream
    void drop() {
        if(eof()) {
            throw std::runtime_error("Unexpected EOF token");
        }
        _tokens.pop();
    }

    /// @brief fetches and removes the current token
    [[nodiscard]] lexer_token pop() {
        lexer_token res = peek();
        drop();
        return res;
    }

    /// @brief attemps retrieving a specific type of token
    /// @tparam Token the token type
    /// @return the current token if it has the right type, or std::nullopt
    template <token_option Token>
    std::optional<Token> try_get() {
        std::optional<Token> res = peek().get<Token>();
        if(res != std::nullopt) { drop(); }
        return res;
    }

    /// @brief attemps retrieving a specific type of token
    /// @tparam Token the token type
    /// @return the current token if it has the right type
    /// @throws std::runtime_error if the current token's type is incorrect
    template <token_option Token>
    Token get() {
        std::optional<Token> res = peek().get<Token>();
        if(res == std::nullopt) {
            throw std::runtime_error("Unexpected token"); // TODO
        }
        drop();
        return res.value();
    }

    template <token_option Token>
    [[nodiscard]] bool discard() {
        if(peek().get<Token>().has_value()) {
            drop();
            return true;
        }
        return false;
    }

    template <token_option Token>
    void expect() {
        if(!discard<Token>()) {
            throw std::runtime_error("Unexpected token"); // TODO
        }
    }

private:
    std::shared_ptr<std::string> _buffer;
    std::queue<lexer_token> _tokens;

    token_stream(std::string&& buf): _buffer(std::make_shared<std::string>(std::forward<std::string>(buf))) {}

    void push(lexer_token tok) { _tokens.push(std::move(tok)); }

    friend token_stream tokenize(std::string data, bool newline);

};


token_stream tokenize(std::string data, bool newline = false);


} // namespace npp



template <>
struct fmt::formatter<npp::lexer_token> {
    bool verbose = true;

    constexpr auto parse(fmt::format_parse_context& ctx) -> fmt::format_parse_context::iterator {
        auto it = ctx.begin();
        if(it != ctx.end() && *it == 'n') {
            verbose = false;
        }
        if(it != ctx.end() && *it != '}') {
            throw fmt::format_error("invalid lexer token format");
        }
        return it;
    }

    auto format(const npp::lexer_token& tok, fmt::format_context& ctx) const -> fmt::format_context::iterator {
        struct format_visitor {
            bool verbose = verbose;
            fmt::format_context& ctx = ctx;

            fmt::format_context::iterator operator()(const npp::tokens::identifier& ident) {
                return verbose ? fmt::format_to(ctx.out(), "{}({})", npp::token_type_name<npp::tokens::identifier>(), ident.name) : fmt::format_to(ctx.out(), "{}", ident.name);
            }
            fmt::format_context::iterator operator()(const npp::tokens::int_cst& cst) {
                return verbose ? fmt::format_to(ctx.out(), "{}({})", npp::token_type_name<npp::tokens::int_cst>(), cst.value) : fmt::format_to(ctx.out(), "{}", cst.value);
            }
            fmt::format_context::iterator operator()(const npp::tokens::plus&) {
                return verbose ? fmt::format_to(ctx.out(), "{}", npp::token_type_name<npp::tokens::plus>()) : fmt::format_to(ctx.out(), "+");
            }
            fmt::format_context::iterator operator()(const npp::tokens::minus&) {
                return verbose ? fmt::format_to(ctx.out(), "{}", npp::token_type_name<npp::tokens::minus>()) : fmt::format_to(ctx.out(), "-");
            }
            fmt::format_context::iterator operator()(const npp::tokens::mul&) {
                return verbose ? fmt::format_to(ctx.out(), "{}", npp::token_type_name<npp::tokens::mul>()) : fmt::format_to(ctx.out(), "*");
            }
            fmt::format_context::iterator operator()(const npp::tokens::div&) {
                return verbose ? fmt::format_to(ctx.out(), "{}", npp::token_type_name<npp::tokens::div>()) : fmt::format_to(ctx.out(), "/");
            }
            fmt::format_context::iterator operator()(const npp::tokens::eq&) {
                return verbose ? fmt::format_to(ctx.out(), "{}", npp::token_type_name<npp::tokens::eq>()) : fmt::format_to(ctx.out(), "==");
            }
            fmt::format_context::iterator operator()(const npp::tokens::ne&) {
                return verbose ? fmt::format_to(ctx.out(), "{}", npp::token_type_name<npp::tokens::ne>()) : fmt::format_to(ctx.out(), "!=");
            }
            fmt::format_context::iterator operator()(const npp::tokens::gt&) {
                return verbose ? fmt::format_to(ctx.out(), "{}", npp::token_type_name<npp::tokens::gt>()) : fmt::format_to(ctx.out(), ">");
            }
            fmt::format_context::iterator operator()(const npp::tokens::lt&) {
                return verbose ? fmt::format_to(ctx.out(), "{}", npp::token_type_name<npp::tokens::lt>()) : fmt::format_to(ctx.out(), "<");
            }
            fmt::format_context::iterator operator()(const npp::tokens::ge&) {
                return verbose ? fmt::format_to(ctx.out(), "{}", npp::token_type_name<npp::tokens::ge>()) : fmt::format_to(ctx.out(), ">=");
            }
            fmt::format_context::iterator operator()(const npp::tokens::le&) {
                return verbose ? fmt::format_to(ctx.out(), "{}", npp::token_type_name<npp::tokens::le>()) : fmt::format_to(ctx.out(), "<=");
            }
            fmt::format_context::iterator operator()(const npp::tokens::eol&) {
                return verbose ? fmt::format_to(ctx.out(), "{}", npp::token_type_name<npp::tokens::eol>()) : fmt::format_to(ctx.out(), "<eol>");
            }
            fmt::format_context::iterator operator()(const npp::tokens::eof&) {
                return verbose ? fmt::format_to(ctx.out(), "{}", npp::token_type_name<npp::tokens::eof>()) : fmt::format_to(ctx.out(), "<eof>");
            }
        } v;
        return std::visit(v, tok.raw());
    }
};
