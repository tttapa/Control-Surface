#pragma once

#include <stddef.h>

// ------------------------------- TYPE_TRAITS ------------------------------ //

#ifdef __AVR__

template <class Tp, Tp v>
struct integral_constant {
    static constexpr Tp value = v;
    typedef Tp value_type;
    typedef integral_constant type;
    constexpr operator value_type() const { return value; }
};

typedef integral_constant<bool, true> true_type;
typedef integral_constant<bool, false> false_type;

template <bool, class Tp = void>
struct enable_if {};
template <class Tp>
struct enable_if<true, Tp> {
    typedef Tp type;
};
template <class Tp, class Up>
struct is_same : public false_type {};
template <class Tp>
struct is_same<Tp, Tp> : public true_type {};
template <class Tp>
struct remove_reference {
    typedef Tp type;
};
template <class Tp>
struct remove_reference<Tp &> {
    typedef Tp type;
};

#else

#include <type_traits>
using std::enable_if;
using std::is_same;
using std::remove_reference;

#endif

// -------------------------------------------------------------------------- //

template <class T, size_t N>
struct Array {
    T data[N];

    T &operator[](size_t index) { return data[index]; }
    const T &operator[](size_t index) const { return data[index]; }
    T *begin() { return &data[0]; }
    const T *begin() const { return &data[0]; }
    T *end() { return &data[N]; }
    const T *end() const { return &data[N]; }
};

// -------------------------------------------------------------------------- //

template <class T, size_t nb_rows, size_t nb_cols>
using Array2D = Array<Array<T, nb_cols>, nb_rows>;