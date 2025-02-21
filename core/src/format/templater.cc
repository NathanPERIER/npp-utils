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


std::optional<std::tuple<std::string_view, size_t>> attempt_unescape(std::string_view buf, size_t pos) {
    switch(buf[pos]) {
        case variable_begin: {
            pos++;
            if(pos < buf.size() && buf[pos] == variable_begin) {
                return std::tuple<std::string_view, size_t>("{", pos+1);
            }
            return std::nullopt;
        }
        case variable_end: {
            pos++;
            if(pos >= buf.size() || buf[pos] != variable_end) {
                throw std::runtime_error("Unexpected variable block end in format pattern");
            }
            return std::tuple<std::string_view, size_t>("}", pos+1);
        }
    }
    throw npp::unreachable();
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
    size_t begin = 0;
    size_t cursor = 0;
    std::string_view buf(*_buffer);

    while(cursor < buf.size()) {
        cursor = buf.find_first_of(special_chars, begin);
        if(cursor > begin) {
            if(cursor == std::string_view::npos) {
                add_raw_text(buf.substr(begin));
                break;
            }
            add_raw_text(buf.substr(begin, cursor - begin));
        }

        // TODO could be more optimised
        std::optional<std::tuple<std::string_view, size_t>> unescape_res = ::attempt_unescape(buf, cursor);
        if(unescape_res.has_value()) {
            add_raw_text(std::get<0>(unescape_res.value()));
            begin = std::get<1>(unescape_res.value());
            continue;
        }

        begin = cursor + 1;
        cursor = ::parse_variable(buf, begin);
        const std::string_view variable_name = buf.substr(begin, cursor - begin);
        add_variable_block(variable_name);
        begin = cursor + 1;
    }
}

void format_pattern::add_raw_text(std::string_view raw) {
    _pieces.emplace_back(npp::detail::format_raw_text {
        .text = raw
    });
}

void format_pattern::add_variable_block(std::string_view variable) {
    _pieces.emplace_back(npp::detail::format_replace_variable {
        .variable_name = variable
    });
    auto it = _variable_indexes.find(variable);
    if(it == _variable_indexes.end()) {
        it = _variable_indexes.insert({ std::string(variable), std::vector<size_t>() }).first;
    }
    it->second.push_back(_pieces.size() - 1);
}

void format_pattern::specialise(const variable_repository& vars) {
    vars.for_each([this](std::string_view var, std::string_view val) {
        const auto it = _variable_indexes.find(var);
        if(it == _variable_indexes.end()) {
            return;
        }
        const npp::lazy_string s = std::string(val);
        for(size_t i: it->second) {
            _pieces[i] = npp::detail::format_raw_text { .text = s };
        }
        _variable_indexes.erase(it);
    });
}

format_pattern format_pattern::specialised(const variable_repository& vars) const {
    format_pattern res(*this);
    res.specialise(vars);
    return res;
}

std::string format_pattern::format(const variable_repository& vars) const {
    std::string res;
    for(const auto& piece: _pieces) {
        if(std::holds_alternative<npp::detail::format_raw_text>(piece)) {
            res.append(std::get<npp::detail::format_raw_text>(piece).text.view());
            continue;
        }
        assert(std::holds_alternative<npp::detail::format_replace_variable>(piece));
        const std::string_view variable_name = std::get<npp::detail::format_replace_variable>(piece).variable_name;
        const std::optional<std::string_view> value = vars.get(variable_name);
        if(!value.has_value()) {
            throw std::runtime_error(fmt::format("No value for variable {}", variable_name));
        }
        res.append(value.value());
    }
    return res;
}

} // namespace npp
