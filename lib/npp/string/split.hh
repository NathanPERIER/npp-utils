
#pragma once

#include <string>

#include <fmt/format.h>


namespace npp {

class split {

public:
    split(std::string_view view, const std::string& sep): _view(view), _separator(sep) {}

    class iterator {

    public:
        // iterator traits
        using difference_type = std::ptrdiff_t;
        using value_type = std::string_view;
        using pointer = const std::string_view*;
        using reference = const std::string_view&;
        using iterator_category = std::forward_iterator_tag;

        static iterator make_begin(std::string_view view, std::string_view sep) { return iterator(view, sep, 0); }
        static iterator make_end(std::string_view view, std::string_view sep)   { return iterator(view, sep);    }

        iterator& operator++() {
            if(_end == std::string_view::npos) {
                _begin = _end;
                return *this;
            }
            _begin = _end + _sep.size();
            _end = next_pos(_view, _sep, _begin);
            return *this;
        }
        iterator operator++(int) { iterator res = *this; ++(*this); return res; }

        bool operator==(const iterator& it) const { 
            return _view.data() == it._view.data() 
                && _sep.data()  == it._sep.data()
                && _begin       == it._begin
                && _end         == it._end;
        }
        bool operator!=(const iterator& it) const { return !(*this == it); }

        std::string_view operator*() {
            if(_end != std::string_view::npos) {
                return _view.substr(_begin, _end - _begin); 
            }
            if(_begin < _view.size()) {
                return _view.substr(_begin);
            }
            return std::string_view();
        }

    protected:
        iterator(std::string_view view, std::string_view sep, std::size_t pos): _view(view), _sep(sep), _begin(pos), _end(next_pos(_view, _sep, _begin)) {}
        iterator(std::string_view view, std::string_view sep): _view(view), _sep(sep), _begin(std::string_view::npos), _end(std::string_view::npos) {}

    private:
        const std::string_view _view;
        const std::string_view _sep;
        std::size_t _begin;
        std::size_t _end;

        static std::size_t next_pos(std::string_view view, std::string_view sep, std::size_t pos) {
            if(pos >= view.size()) {
                return std::string_view::npos;
            }
            return view.find(sep, pos);
        }

    };

    iterator begin() { return iterator::make_begin(_view, _separator); }
    iterator end()   { return iterator::make_end(_view, _separator);   }

private:
    std::string_view _view;
    std::string _separator;

};

} // namespace npp
