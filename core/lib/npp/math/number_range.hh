
#pragma once

#include <compare>
#include <vector>

#include <npp/math/algebraic_ring.hh>


namespace npp {

template <algebraic_ring Ring>
class number_range {
public:
    using element_type = Ring::element_type;

    number_range(element_type begin, element_type end): _begin(begin), _end(end) {
        assert(_begin <= _end);
    }
    number_range(): _begin(Ring::zero()), _end(Ring::zero()) {}

    element_type begin() const { return _begin; }
    element_type size() const { return _end - _begin; }
    element_type end() const { return _end; }

    bool empty() const { return _begin == _end; }

    bool contains(element_type elt) const {
        return (begin() <= elt) && (elt <= end());
    }

    bool includes(const number_range<Ring>& range) const {
        return (begin() <= range.begin()) && (range.end() <= end());
    }

    bool operator==(const number_range<Ring>&) const = default;

private:
    element_type _begin;
    element_type _end;
};


template <algebraic_ring Ring>
class number_multirange {
public:
    using range_type = number_range<Ring>;
    using element_type = Ring::element_type;

    bool empty() const { return _ranges.empty(); }

    const std::vector<range_type>& ranges() const { return _ranges; }

    bool contains(element_type elt) const {
        const auto it = first_candidate(elt);
        if(it == _ranges.end()) {
            return false;
        }
        return elt <= it->end();
    }

    bool includes(const range_type& range) const {
        const auto it = first_candidate(range.begin());
        if(it == _ranges.end()) {
            return false;
        }
        return range.end() <= it->end();
    }

    bool includes(const number_multirange<Ring>& multirange) const {
        if(empty()) {
            return multirange.empty();
        }
        const auto it = _ranges.begin();
        const auto other_it = multirange._ranges.begin();
        while(other_it != multirange._ranges.end()) {
            while(other_it->begin() < it->begin()) {
                it++;
                if(it == _ranges.end()) {
                    return false;
                }
            }
            if(it->end() < other_it->end()) {
                return false;
            }
            other_it++;
        }
        return true;
    }

    bool operator==(const number_multirange<Ring>&) const = default;


    void insert(element_type elt) {
        if(_ranges.empty()) {
            _ranges.emplace_back(elt, Ring::one());
        }
        auto it = first_candidate(elt);
        if(it == _ranges.end()) {
            it = _ranges.begin();
            if(elt == it->begin() + Ring::one()) {
                *it = range_type(it->begin() - Ring::one(), it->end());
            } else {
                _ranges.emplace(it, elt, elt + Ring::one());
            }
            return;
        }
        if(elt <= it->end()) {
            return;
        }
        auto next_it = it + 1;
        const bool is_next_start = (next_it != _ranges.end()) && (next_it->begin() == it->end() + Ring::one());
        if(elt == it->end() + Ring::one()) {
            if(is_next_start) {
                *it = range_type(it->begin(), next_it->end());
                _ranges.erase(next_it);
            } else {
                *it = range_type(it->begin(), it->end() + Ring::one());
            }
            return;
        }
        if(is_next_start) {
            *next_it = range_type(next_it->begin() - Ring::one(), next_it->end());
        } else {
            _ranges.emplace_back(elt, elt + Ring::one());
        }
    }

    // TODO insertion with ranges and multiranges

    void erase(element_type elt) {
        if(_ranges.empty()) {
            return;
        }
        auto it = first_candidate(elt);
        if(it == _ranges.end() || elt > it->end()) {
            return;
        }
        if(elt == it->begin()) {
            *it = range_type(it->begin() + Ring::one(), it->end());
            return;
        }
        if(elt == it->end()) {
            *it = range_type(it->begin(), it->end() - Ring::one());
            return;
        }
        range_type first_part(it->begin(), elt - Ring::one());
        *it = range_type(elt + Ring::one(), it->end());
        _ranges.insert(it, first_part);
    }

    // TODO erase with ranges and multiranges

    // TODO intersection

private:
    using range_vector = std::vector<range_type>;
    range_vector _ranges;

    range_vector::iterator first_candidate(const element_type& elt) {
        return std::find_if(_ranges.begin(), _ranges.end(), [&elt](const range_type& range) {
            return range.begin() <= elt;
        });
    }

    range_vector::const_iterator first_candidate(const element_type& elt) const {
        return std::find_if(_ranges.begin(), _ranges.end(), [&elt](const range_type& range) {
            return range.begin() <= elt;
        });
    }
};

} // namespace npp
