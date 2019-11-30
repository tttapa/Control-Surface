#pragma once

#include <AH/Settings/Warnings.hpp>
AH_DIAGNOSTIC_WERROR() // Enable errors on warnings

#include <AH/STL/type_traits>
#include <AH/STL/utility> // std::forward

BEGIN_AH_NAMESPACE

/// @addtogroup    AH_Containers
/// @{

/**
 * @brief   Very basic smart pointer. Doesn't support array types.
 * 
 * @tparam  T
 *          The type of the pointee.
 */
template <class T>
class UniquePtr {
  public:
    UniquePtr() = default;
    explicit UniquePtr(T *p) : p(p) {}
    UniquePtr(const UniquePtr &) = delete;
    template <class U>
    UniquePtr(UniquePtr<U> &&r) {
        reset(r.release());
    }

    ~UniquePtr() { delete p; }

    UniquePtr &operator=(const UniquePtr &) = delete;
    UniquePtr<T> &operator=(UniquePtr<T> &&r) {
        reset(r.release());
        return *this;
    }

    void reset(T *newP = nullptr) {
        T *old_ptr = p;
        p = newP;
        delete old_ptr;
    }

    T *release() {
        T *old_ptr = p;
        p = nullptr;
        return old_ptr;
    }

    explicit operator bool() const { return p != nullptr; }

    T *get() { return p; }
    const T *get() const { return p; }

    T *operator->() { return p; }
    const T *operator->() const { return p; }
    T &operator*() { return *p; }
    const T &operator*() const { return *p; }

  private:
    T *p = nullptr;
};

template <typename T, typename... Args>
UniquePtr<T> MakeUnique(Args &&... args) {
    return UniquePtr<T>(new T(std::forward<Args>(args)...));
}

/// @}

END_AH_NAMESPACE

AH_DIAGNOSTIC_POP()
