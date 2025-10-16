
#pragma once

#include <algorithm>
#include <optional>
#include <string>
#include <unordered_set>
#include <variant>
#include <vector>

#include "npp/string/lazy.hh"
#include "npp/typing/functional.hh"
#include "npp/typing/map.hh"


namespace npp {

class variable_repository {

public:
    void set(std::string_view variable, std::string value) {
        auto it = _variables.find(variable);
        if(it == _variables.end()) {
            _variables.emplace(variable, std::move(value));
            return;
        }
        it->second = std::move(value);
    }

    std::optional<std::string_view> get(std::string_view variable) const {
        auto it = _variables.find(variable);
        if(it == _variables.end()) {
            return std::nullopt;
        }
        return std::string_view(it->second);
    }

    template <npp::void_invocable<std::string_view, std::string_view> F>
    void for_each(F f) const {
        for(const auto& v: _variables) {
            f(v.first, v.second);
        }
    }

private:
    npp::string_umap<std::string> _variables;
};


class format_pattern {

public:
    format_pattern(const std::string& pattern);
    format_pattern(std::string&& pattern);

    format_pattern(const format_pattern&) = default;
    format_pattern(format_pattern&&) = default;
    
    bool has_variables() const { return !_pieces.empty(); }

    std::unordered_set<std::string_view> variables() const;

    std::string format(const variable_repository& vars) const;

private:
    std::shared_ptr<std::string> _buffer;

    struct pattern_piece {
        std::vector<std::string_view> raw_text;
        std::string_view variable;
    };

    std::vector<pattern_piece> _pieces;
    std::vector<std::string_view> _trailing_text;
};

} // namespace npp
