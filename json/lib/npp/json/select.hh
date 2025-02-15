
#pragma once

#include <concepts>
#include <string_view>
#include <variant>
#include <stack>

#include "npp/typing/json.hh"


namespace npp {

struct wildcard {};

using json_select_path_component = std::variant<std::string_view, size_t, wildcard>;


template <json Json, std::invocable<const Json&> Callback>
void json_select(const Json& data, const Callback& cb, std::span<const json_select_path_component> cmpts) {
    if(cmpts.empty()) {
        cb(data);
        return;
    }
    const json_select_path_component& cmpt = cmpts[0];
    if(std::holds_alternative<std::string_view>(cmpt)) {
        if(!data.is_object()) {
            return;
        }
        const auto it = data.find(std::get<std::string_view>(cmpt));
        if(it == data.end()) {
            return;
        }
        json_select(*it, cb, cmpts.subspan(1));
    } else if(std::holds_alternative<size_t>(cmpt)) {
        if(!data.is_array()) {
            return;
        }
        const size_t idx = std::get<size_t>(cmpt);
        if(idx >= data.size()) {
            return;
        }
        json_select(data.at(idx), cb, cmpts.subspan(1));
    } else {
        for(const Json& child: data) {
            json_select(child, cb, cmpts.subspan(1));
        }
    }
}


} // namespace npp
