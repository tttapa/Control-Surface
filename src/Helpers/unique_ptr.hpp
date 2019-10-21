#pragma once

#ifndef DOXYGEN

template <class T>
struct remove_reference {
    typedef T type;
};
template <class T>
struct remove_reference<T &> {
    typedef T type;
};
template <class T>
struct remove_reference<T &&> {
    typedef T type;
};

template <class T>
typename remove_reference<T>::type &&move(T &&t) {
    return static_cast<typename remove_reference<T>::type &&>(t);
}

template <class T, T v>
struct integral_constant {
    constexpr static T value = v;
};

using true_type = integral_constant<bool, true>;
using false_type = integral_constant<bool, false>;

template <class T>
struct is_lvalue_reference : false_type {};
template <class T>
struct is_lvalue_reference<T &> : true_type {};

template <class T>
inline T &&forward(typename remove_reference<T>::type &t) noexcept {
    return static_cast<T &&>(t);
}

template <class T>
inline T &&forward(typename remove_reference<T>::type &&t) noexcept {
    static_assert(!is_lvalue_reference<T>::value,
                  "Can not forward an rvalue as an lvalue.");
    return static_cast<T &&>(t);
}

#endif

/// @addtogroup    Utilities
/// @{

/**
 * @brief   Very basic smart pointer. Doesn't support array types.
 * 
 * @tparam  T
 *          The type of the pointee.
 */
template <class T>
class unique_ptr {
  public:
    unique_ptr() = default;
    explicit unique_ptr(T *p) : p(p) {}
    unique_ptr(const unique_ptr &) = delete;
    template <class U>
    unique_ptr(unique_ptr<U> &&r) {
        reset(r.release());
    }

    ~unique_ptr() { delete p; }

    unique_ptr &operator=(const unique_ptr &) = delete;
    unique_ptr<T> &operator=(unique_ptr<T> &&r) {
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
unique_ptr<T> make_unique(Args &&... args) {
    return unique_ptr<T>(new T(forward<Args>(args)...));
}

/// @}