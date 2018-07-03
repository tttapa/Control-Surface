#ifndef COPY_HPP_
#define COPY_HPP_

#include <stddef.h>

template <class T, class S, size_t N>
void copy(T *dest, const S (&src)[N]) {
    for (size_t i = 0; i < N; i++)
        dest[i] = (T)src[i];
}

#endif // COPY_HPP_