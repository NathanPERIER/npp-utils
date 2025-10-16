#include "npp/format/templater.hh"

#include <cassert>
#include <cctype>

#include "npp/error.hh"


namespace {

constexpr std::string_view special_chars = "{}";
constexpr char variable_begin = special_chars[0];
constexpr char variable_end = special_chars[1];


bool is_variable_begin(char c) {
    return std::isalpha(static_cast<unsigned char>(c)) || c == '_';
}

bool is_variable(char c) {
    return is_variable_begin(c) || std::isdigit(static_cast<unsigned char>(c));
}

size_t parse_variable(std::string_view buf, size_t pos) {
    if(pos >= buf.size()) {
        throw std::runtime_error("Dangling variable block in format pattern");
    }
    if(buf[pos] == variable_end) {
        throw std::runtime_error("Empty variable block in format pattern");
    }
    if(!is_variable_begin(buf[pos])) {
        throw std::runtime_error(fmt::format("Invalid character at start of variable name: {:x}", buf[pos]));
    }
    pos++;
    while(pos < buf.size() && is_variable(buf[pos])) {
        pos++;
    }
    if(pos >= buf.size()) {
        throw std::runtime_error("Dangling variable block in format pattern");
    }
    if(buf[pos] != variable_end) {
        throw std::runtime_error(fmt::format("Invalid character in variable block: {:x}", buf[pos]));
    }
    return pos;
}

} // anonymous namespace



namespace npp {

format_pattern::format_pattern(const std::string& pattern): format_pattern(std::string(pattern)) {}

format_pattern::format_pattern(std::string&& pattern): _buffer(std::make_shared<std::string>(std::forward<std::string>(pattern))) {
    std::string_view buf(*_buffer);
    size_t begin = 0;
    std::vector<std::string_view> current_text;

    while(begin < buf.size()) {
        size_t cursor = buf.find_first_of(special_chars, begin);
        if(cursor == std::string_view::npos) {
            current_text.push_back(buf.substr(begin));
            _trailing_text = std::move(current_text);
            current_text = std::vector<std::string_view>();
            break;
        }

        // If repetition, only include the first occurence and keep parsing text
        if(cursor+1 < buf.size() && buf[cursor] == buf[cursor+1]) {
            current_text.push_back(buf.substr(begin, cursor + 1 - begin));
            begin = cursor + 2;
            continue;
        }

        if(buf[cursor] == ::variable_end) {
            throw std::runtime_error("Unexpected variable block end in format pattern");
        }

        current_text.push_back(buf.substr(begin, cursor - begin));
        begin = cursor + 1;

        cursor = ::parse_variable(buf, begin);
        _pieces.push_back(pattern_piece {
            .raw_text = std::move(current_text),
            .variable = buf.substr(begin, cursor - begin)
        });
        current_text = std::vector<std::string_view>();
        begin = cursor + 1;
    }

    if(!current_text.empty()) {
        _trailing_text = std::move(current_text);
    }
}

std::string format_pattern::format(const variable_repository& vars) const {
    std::string res;
    for(const pattern_piece& piece: _pieces) {
        for(const std::string_view& text: piece.raw_text) {
            res.append(text);
        }
        const std::optional<std::string_view> value = vars.get(piece.variable);
        if(!value.has_value()) {
            throw std::runtime_error(fmt::format("No value for variable {}", piece.variable));
        }
        res.append(value.value());
    }
    for(const std::string_view& text: _trailing_text) {
        res.append(text);
    }
    return res;
}

std::unordered_set<std::string_view> format_pattern::variables() const {
    std::unordered_set<std::string_view> res;
    for(const pattern_piece& piece: _pieces) {
        res.insert(piece.variable);
    }
    return res;
}

} // namespace npp
