#pragma once

#include <stddef.h>

/**
 * @brief   Copy the given source array to the destination.  
 *          If the source and destination types are different,
 *          each element of the source array will be cast to the
 *          destination type. 
 * 
 * @param   dest
 *          A pointer to the destination.
 * @param   src
 *          The source array.
 */
template <class T, class S, size_t N>
void copy(T *dest, const S (&src)[N]) {
    for (size_t i = 0; i < N; i++)
        dest[i] = (T)src[i];
}