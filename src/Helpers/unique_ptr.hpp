#pragma once

/**
 * @brief   Very basic smart pointer. Doesn't support array types.
 * 
 * @tparam  T
 */
template <class T>
class unique_ptr {
  public:
    unique_ptr() = default;
    unique_ptr(T *p) : p(p) {}

    ~unique_ptr() { delete p; }

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