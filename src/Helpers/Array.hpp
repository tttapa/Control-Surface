#pragma once

#include <stddef.h>

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