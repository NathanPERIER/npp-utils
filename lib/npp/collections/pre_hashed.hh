
#pragma once

#include <unordered_map>
#include <unordered_set>

#include "npp/typing/hashable.hh"


// Note: this will be more useful in C++26 since we will be able to use the unordered_map's access operator
//       with any type accepted by the hash function and equality function (in C++20/23 this only works with find)


namespace npp {

template <typename T, class Hash=std::hash<T>>
requires (npp::hashable<T, Hash>)
class pre_hashed {

public:
    template <typename... Args>
    static pre_hashed<T, Hash> make(Args&&... args) {
        return pre_hashed<T, Hash>(T(std::forward<Args>(args)...));
    }

    const T& value() const { return _value; }
    size_t hash() const { return _hash; }

private:
    T _value;
    size_t _hash;

    pre_hashed(T&& value): _value(std::forward<T>(value)), _hash(Hash{}(_value)) {}
};


struct hash_index { size_t hash; };

} // namespace npp


namespace npp::detail {

template <class Hash, npp::hashable<Hash> Hashable>
struct pre_hash_helper {
    size_t operator()(const Hashable& value) const {
        return Hash{}(value);
    }
    size_t operator()(const npp::pre_hashed<Hashable, Hash>& hashed) const {
        return hashed.hash();
    }
    size_t operator()(const npp::hash_index& index) const {
        return index.hash;
    }
};


template <class Hash, npp::hashable<Hash> Hashable, class KeyEqual>
struct pre_hash_eq_helper {
    bool operator()(const npp::pre_hashed<Hashable, Hash>& h1, const npp::pre_hashed<Hashable, Hash>& h2) const {
        return KeyEqual{}(h1.value(), h2.value());
    }
    bool operator()(const npp::pre_hashed<Hashable, Hash>& h, const Hashable& v) const {
        return KeyEqual{}(h.value(), v);
    }
    bool operator()(const Hashable& v, const npp::pre_hashed<Hashable, Hash>& h) const {
        return KeyEqual{}(h.value(), v);
    }
};

} // namespace npp::detail


namespace npp {

template <typename Hashable, class T, class Hash=std::hash<Hashable>, class KeyEqual=std::equal_to<Hashable>>
requires (hashable<Hashable, Hash>)
using prehash_umap = std::unordered_map<npp::pre_hashed<Hashable, Hash>, T, npp::detail::pre_hash_helper<Hash, Hashable>, npp::detail::pre_hash_eq_helper<Hash, Hashable, KeyEqual>>;

template <typename Hashable, class Hash=std::hash<Hashable>, class KeyEqual=std::equal_to<Hashable>>
requires (hashable<Hashable, Hash>)
using prehash_uset = std::unordered_set<npp::pre_hashed<Hashable, Hash>, npp::detail::pre_hash_helper<Hash, Hashable>, npp::detail::pre_hash_eq_helper<Hash, Hashable, KeyEqual>>;

} // namespace npp



template <class Hash, npp::hashable<Hash> Hashable>
struct std::hash<npp::pre_hashed<Hashable, Hash>> {
    size_t operator()(const npp::pre_hashed<Hashable, Hash>& hashed) {
        return hashed.hash();
    }
};
