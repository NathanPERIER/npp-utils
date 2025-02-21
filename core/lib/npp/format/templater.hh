
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


namespace npp::detail {

struct format_replace_variable {
    std::string_view variable_name;
};

struct format_raw_text {
    npp::lazy_string text;
};

using pattern_piece = std::variant<format_raw_text, format_replace_variable>;

} // namespace npp::detail


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
    
    bool has_variables() const { return !_variable_indexes.empty(); }

    std::unordered_set<std::string_view> variables() const {
        std::unordered_set<std::string_view> res;
        for(const auto& var: _variable_indexes) {
            res.insert(var.first);
        }
        return res;
    }

    void specialise(const variable_repository& vars);
    format_pattern specialised(const variable_repository& vars) const;

    std::string format(const variable_repository& vars) const;

private:
    std::shared_ptr<std::string> _buffer;
    std::vector<npp::detail::pattern_piece> _pieces;
    npp::string_umap<std::vector<size_t>> _variable_indexes;

    void add_raw_text(std::string_view raw);
    void add_variable_block(std::string_view variable);

};

} // namespace npp
