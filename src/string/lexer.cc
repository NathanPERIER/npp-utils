#include "npp/string/lexer.hh"


namespace {

bool is_digit(char c)  { return '0' <= c && c <= '9'; }
bool is_letter(char c) { return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z'); }


class stream_tokenizer {

public:
    stream_tokenizer(std::string_view data): _data(data), _pos({0, 1, 1}) {}

    size_t index() const { return _pos.idx; }

    bool eof() const { return _pos.idx >= _data.length(); }
    npp::lexer_token::position pos() const { return _pos; }

    char peek() const {
        if(eof()) {
            throw std::runtime_error("Unexpected EOF");
        }
        return _data[_pos.idx];
    }

    void skip() {
        if(eof()) {
            throw std::runtime_error("Unexpected EOF");
        }
        _pos.chr++;
        _pos.idx++;
    }

    char pop() {
        char res = peek();
        skip();
        return res;
    }

    void expect(char c) {
        char top = peek();
        if(top != c) {
            throw std::runtime_error(fmt::format("Expected character '{}', got '{}'", c, top));
        }
        skip();
    }

    void skip_spaces() {
        static constexpr std::string_view skip_chars = " \t\r\n";
        while(_pos.idx < _data.size() && skip_chars.find(_data[_pos.idx]) != std::string_view::npos) {
            if(_data[_pos.idx] == '\n') {
                _pos.chr = 1;
                _pos.line++;
            } else {
                _pos.chr++;
            }
            _pos.idx++;
        }
    }

    void skip_spaces_until_eol() {
        static constexpr std::string_view skip_chars = " \t\r";
        while(_pos.idx < _data.size() && skip_chars.find(_data[_pos.idx]) != std::string_view::npos) {
            _pos.chr++;
            _pos.idx++;
        }
    }

    std::string_view substr(size_t start_idx) const {
        if(start_idx >= _pos.idx) {
            throw std::runtime_error(fmt::format("Bad substring range [{}:{}]", start_idx, _pos.idx));
        }
        return _data.substr(start_idx, _pos.idx - start_idx);
    }

    npp::lexer_token parse_token();

private:
    std::string_view _data;
    npp::lexer_token::position _pos;

};


npp::lexer_token parse_identifier(stream_tokenizer& tokenizer, const npp::lexer_token::position& start_pos) {
    static auto is_continuation = [](char c) -> bool { return is_letter(c) || is_digit(c) || c == '_'; };
    while(!tokenizer.eof() && is_continuation(tokenizer.peek())) {
        tokenizer.pop();
    }
    return npp::lexer_token(npp::tokens::identifier{ .name = tokenizer.substr(start_pos.idx) }, start_pos, tokenizer.pos());
}

npp::lexer_token parse_number(stream_tokenizer& tokenizer, const npp::lexer_token::position& start_pos) {
    while(!tokenizer.eof() && is_digit(tokenizer.peek())) {
        tokenizer.pop();
    }
    std::string_view repr = tokenizer.substr(start_pos.idx);
    return npp::lexer_token(npp::tokens::int_cst{ .value = std::strtoll(repr.data(), nullptr, 10) }, start_pos, tokenizer.pos());
}


/// @warning assumes that the index is valid for the string
npp::lexer_token stream_tokenizer::parse_token() {
    npp::lexer_token::position start_pos = pos();
    char c = pop();
    switch(c) {
        case '+': return npp::lexer_token(npp::tokens::plus{}, start_pos);
        case '-': return npp::lexer_token(npp::tokens::minus{}, start_pos);
        case '*': return npp::lexer_token(npp::tokens::mul{}, start_pos);
        case '/': return npp::lexer_token(npp::tokens::div{}, start_pos);
        case '=': expect('='); return npp::lexer_token(npp::tokens::eq{}, start_pos, 2);
        case '!': expect('='); return npp::lexer_token(npp::tokens::ne{}, start_pos, 2);
        case '<': switch (peek()) {
            case '=': skip(); return npp::lexer_token(npp::tokens::le{}, start_pos, 2);
            default:          return npp::lexer_token(npp::tokens::lt{}, start_pos, 2);
        };
        case '>': switch (peek()) {
            case '=': skip(); return npp::lexer_token(npp::tokens::ge{}, start_pos, 2);
            default:          return npp::lexer_token(npp::tokens::gt{}, start_pos, 2);
        };
        // TODO case '"': // parse string
    }
    if(is_digit(c)) {
        return parse_number(*this, start_pos);
    }
    if(is_letter(c) || c == '_') {
        return parse_identifier(*this, start_pos);
    }
    throw std::runtime_error(fmt::format("Unexpected character '{}'", c));
}

} // anonymous namespace


namespace npp {

token_stream tokenize(std::string buf, bool newline) {
    token_stream tokens(std::move(buf));
    ::stream_tokenizer tokenizer(*tokens._buffer);

    while(true) {
        if(newline) {
            tokenizer.skip_spaces_until_eol();
            if(!tokenizer.eof() && tokenizer.peek() == '\n') {
                tokens.push(lexer_token(tokens::eol{}, tokenizer.pos()));
                tokenizer.skip_spaces();
            }
        } else {
            tokenizer.skip_spaces();
        }
        if(tokenizer.eof()) {
            tokens.push(lexer_token(tokens::eof{}, tokenizer.pos()));
            break;
        }
        tokens.push(tokenizer.parse_token());
    }

    return tokens;
}

} // namespace npp
