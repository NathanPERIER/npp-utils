
#pragma once

#include <concepts>
#include <functional>
#include <memory>
#include <unordered_map>



namespace npp::detail {

template <typename T>
requires (std::movable<T> && std::copyable<T>)
class callback_holder {

public:
    callback_holder() {}

    callback_holder(callback_holder<T>&&) = delete;
    callback_holder<T>& operator=(callback_holder<T>&&) = delete;
    
    callback_holder(const callback_holder<T>&) = delete;
    callback_holder<T>& operator=(const callback_holder<T>&) = delete;

    using callback_type = std::function<void(const T&)>;

    std::size_t add_callback(callback_type&& cb) {
        _prev_id++;
        _callbacks.insert({_prev_id, std::forward<callback_type>(cb)});
        return _prev_id;
    }

    void disarm(std::size_t id) {
        const auto it = _callbacks.find(id);
        if(it == _callbacks.end()) {
            return;
        }
        _callbacks.erase(it);
    }

    void trigger_update(const T& value) {
        for(const auto& e: _callbacks) {
            e.second(value);
        }
    }

private:
    std::size_t _prev_id = 0;
    std::unordered_map<std::size_t, callback_type> _callbacks;

};

} // namespace npp::detail

namespace npp {

template <typename T>
requires (std::movable<T> && std::copyable<T>)
class [[nodiscard]] subscription {
    
public:
    subscription(): _holder(nullptr) {}
    subscription(std::shared_ptr<npp::detail::callback_holder<T>> holder, std::size_t id): _holder(holder), _id(id) {}

    subscription(subscription<T>&&) = default;
    subscription<T>& operator=(subscription<T>&&) = default;
    
    subscription(const subscription<T>&) = delete;
    subscription<T>& operator=(const subscription<T>&) = delete;

    bool is_active() const { return _holder != nullptr; }

    void unsubscribe() {
        if(_holder == nullptr) {
            return;
        }
        _holder->disarm(_id);
        _holder = nullptr;
    }

    ~subscription() {
        unsubscribe();
    }

private:
    std::shared_ptr<npp::detail::callback_holder<T>> _holder;
    std::size_t _id;

};


template <typename T>
requires (std::movable<T> && std::copyable<T>)
class observable {

public:
    observable(T&& value): _value(std::make_shared<T>(std::forward<T>(value))), _holder(std::make_shared<npp::detail::callback_holder<T>>()) {}

    const T& operator*() const { return *_value; }
    const T* operator->() const { return _value.get(); }

    template <typename... Args>
    requires (std::constructible_from<T, Args...>)
    static observable<T> make(Args&&... args) {
        return observable<T>(T(std::forward<Args>(args)...));
    }


    std::shared_ptr<const T> get() const { return _value; }

    std::shared_ptr<T> working_copy() {
        if(_working_copy == nullptr) {
            _working_copy = std::make_shared<T>(*_value);
        }
        return _working_copy;
    }

    void abort_changes() {
        _working_copy = nullptr;
    }

    bool has_changes() const {
        if(_working_copy == nullptr) {
            return false;
        }
        if constexpr (std::equality_comparable<T>) {
            return (*_working_copy != *_value);
        }
        return true;
    }

    void commit_changes() {
        if(!has_changes()) {
            return;
        }
        *_value = std::move(*_working_copy);
        _working_copy = nullptr;
        _holder->trigger_update(*_value);
    }

    void set(T&& value) {
        *_value = std::forward<T>(value);
        _holder->trigger_update(*_value);
    }

    subscription<T> subscribe(npp::detail::callback_holder<T>::callback_type callback, bool call_now = false) {
        if(call_now) {
            callback(*_value);
        }
        const std::size_t id = _holder->add_callback(std::move(callback));
        return subscription<T>(_holder, id);
    }

private:
    std::shared_ptr<T> _value;
    std::shared_ptr<T> _working_copy = nullptr;
    std::shared_ptr<npp::detail::callback_holder<T>> _holder;

};

} // namespace npp

