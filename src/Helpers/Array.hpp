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

    bool operator==(const Array<T, N> &rhs) const {
        if (this == &rhs)
            return true;
        for (size_t i = 0; i < N; i++)
            if ((*this)[i] != rhs[i])
                return false;
        return true;
    }

    bool operator!=(const Array<T, N> &rhs) const { return !(*this == rhs); }

    constexpr static size_t length = N;
};
