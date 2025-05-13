
#pragma once

#include <optional>
#include <type_traits>

#include "npp/typing/map.hh"
#include "npp/typing/printable.hh"


namespace npp {

template <typename E>
requires(std::is_enum_v<E> && formattable<E>)
class enum_parser {

public:
    enum_parser(std::initializer_list<E> members) {
        for(const E& mem: members) {
            const bool inserted = _deserialise_mapping.insert({ fmt::format("{}", mem), mem }).second;
            if(!inserted) {
                throw std::runtime_error(fmt::format("Enum collision detected for key {}", mem));
            }
        }
    }

    std::optional<E> parse(std::string_view repr) const {
        const auto it = _deserialise_mapping.find(repr);
        if(it == _deserialise_mapping.end()) {
            return std::nullopt;
        }
        return it->second;
    }

private:
    string_umap<E> _deserialise_mapping;

};

} // namespace npp
