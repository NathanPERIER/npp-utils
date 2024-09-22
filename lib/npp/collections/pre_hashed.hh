
#pragma once

#include <unordered_map>

#include "npp/typing/hashable.hh"


// Note: this will be more useful in C++26 since we will be able to use the unordered_map's access operator
//       with any type accepted by the hash function and equality function (in C++20/23 this only works with find)


namespace npp {

template <npp::hashable Hashable>
class pre_hashed {

public:
    template <typename... Args>
    static pre_hashed<Hashable> make(Args&&... args) {
        return pre_hashed<Hashable>(Hashable(std::forward<Args>(args)...));
    }

    const Hashable& value() const { return _value; }
    size_t hash() const { return _hash; }


private:
    Hashable _value;
    size_t _hash;

    pre_hashed(Hashable&& value): _value(std::forward<Hashable>(value)), _hash(std::hash<Hashable>{}(_value)) {}
};


struct hash_index { size_t hash; };

} // namespace npp


namespace npp::detail {

template <npp::hashable Hashable>
struct pre_hash_helper {
    size_t operator()(const Hashable& value) const {
        return std::hash<Hashable>{}(value);
    }
    size_t operator()(const npp::pre_hashed<Hashable>& hashed) const {
        return hashed.hash();
    }
    size_t operator()(const npp::hash_index& index) const {
        return index.hash;
    }
};


template <npp::hashable Hashable, class KeyEqual>
struct pre_hash_eq_helper {
    bool operator()(const npp::pre_hashed<Hashable>& h1, const npp::pre_hashed<Hashable>& h2) const {
        return KeyEqual{}(h1.value(), h2.value());
    }
    bool operator()(const npp::pre_hashed<Hashable>& h, const Hashable& v) const {
        return KeyEqual{}(h.value(), v);
    }
    bool operator()(const Hashable& v, const npp::pre_hashed<Hashable>& h) const {
        return KeyEqual{}(h.value(), v);
    }
};

} // namespace npp::detail


namespace npp {

template <hashable Hashable, class T, class KeyEqual=std::equal_to<Hashable>>
using prehash_umap = std::unordered_map<npp::pre_hashed<Hashable>, T, npp::detail::pre_hash_helper<Hashable>, npp::detail::pre_hash_eq_helper<Hashable, KeyEqual>>;

} // namespace npp



template <npp::hashable Hashable>
struct std::hash<npp::pre_hashed<Hashable>> {
    size_t operator()(const npp::pre_hashed<Hashable>& hashed) {
        return hashed.hash();
    }
};
