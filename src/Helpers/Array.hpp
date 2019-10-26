/* ✔ */

#pragma once

#include <Helpers/Error.hpp>
#include <stddef.h>

/// @addtogroup Containers
/// @{

/**
 * @brief   An array wrapper for easy copying, comparing, and iterating.
 * 
 * @tparam  T
 *          The type of the elements in the array.
 * @tparam  N 
 *          The number of elements in the array.
 */
template <class T, size_t N>
struct Array {
    T data[N];
    using type = T;
    constexpr static size_t length = N;

    /**
     * @brief   Get the element at the given index.
     * 
     * @note    Bounds checking is performed. If fatal errors are disabled, the
     *          last element is returned if the index is out of bounds. 
     * 
     * @param   index
     *          The (zero-based) index of the element to return.
     */
    T &operator[](size_t index) {
        if (index >= N) { // TODO
            ERROR(F("Index out of bounds: ") << index << F(" ≥ ") << N, 0xEDED);
            index = N - 1;
        }
        return data[index];
    }

    /**
     * @brief   Get the element at the given index.
     * 
     * @note    Bounds checking is performed. If fatal errors are disabled, the
     *          last element is returned if the index is out of bounds. 
     * 
     * @param   index
     *          The (zero-based) index of the element to return.
     */
    const T &operator[](size_t index) const {
        if (index >= N) { // TODO
            ERROR(F("Index out of bounds: ") << index << F(" ≥ ") << N, 0xEDED);
            index = N - 1;
        }
        return data[index];
    }

    /**
     * @brief   Get a pointer to the first element.
     */
    T *begin() { return &data[0]; }

    /**
     * @brief   Get a pointer to the first element.
     */
    const T *begin() const { return &data[0]; }

    /**
     * @brief   Get a pointer to the memory beyond the array.
     */
    T *end() { return &data[N]; }

    /**
     * @brief   Get a pointer to the memory beyond the array.
     */
    const T *end() const { return &data[N]; }

    /**
     * @brief   Check the equality of all elements in two arrays.
     * 
     * @param   rhs 
     *          The array to compare this array to.
     */
    bool operator==(const Array<T, N> &rhs) const {
        if (this == &rhs)
            return true;
        for (size_t i = 0; i < N; i++)
            if ((*this)[i] != rhs[i])
                return false;
        return true;
    }

    /**
     * @brief   Check the inequality of all elements in two arrays.
     * 
     * @param   rhs 
     *          The array to compare this array to.
     */
    bool operator!=(const Array<T, N> &rhs) const { return !(*this == rhs); }
};

/// @}