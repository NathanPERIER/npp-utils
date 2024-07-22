
#pragma once

#include <memory>


namespace npp {

template <typename T>
class copy_ptr {

public:
    copy_ptr(): _data(nullptr) {}
    copy_ptr(std::nullptr_t): copy_ptr() {}
    copy_ptr(std::shared_ptr<T> ptr): _data(std::move(ptr)) {}
    copy_ptr(std::unique_ptr<T> ptr): _data(std::move(ptr)) {}

    copy_ptr(copy_ptr<T>&& ptr) = default;
    copy_ptr<T>& operator=(copy_ptr<T>&& ptr) = default;

    copy_ptr(const copy_ptr<T>& ptr): _data((ptr == nullptr) ? nullptr : std::make_shared<T>(*ptr._data)) {}
    copy_ptr<T>& operator=(const copy_ptr<T>& ptr) {
        _data = (ptr == nullptr) ? nullptr : std::make_shared<T>(*ptr._data);
        return *this;
    }

    copy_ptr<T> share() const { return copy_ptr<T>(_data); }

    T& operator*()  const noexcept { return *_data;      }
    T* operator->() const noexcept { return _data.get(); }

    bool operator==(const copy_ptr<T>& ptr) const { return (_data == ptr._data); }
    bool operator==(nullptr_t)              const { return (_data == nullptr);   }

private:
    std::shared_ptr<T> _data;

};


template <typename T>
bool operator==(nullptr_t, const copy_ptr<T>& ptr) {
    return ptr == nullptr;
}


template <typename T, typename... Args>
copy_ptr<T> make_copy_ptr(Args&&... args) {
    return copy_ptr<T>(std::make_shared<T>(std::forward<Args>(args)...));
}

} // namespace npp
