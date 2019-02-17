/* ✔ */

#pragma once

#include <stddef.h>

#if __cplusplus >= 201400L
#define USE_CONSTEXPR_ARRAY_HELPERS constexpr
#else
#define USE_CONSTEXPR_ARRAY_HELPERS
#endif

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
    constexpr static size_t length = N;

    /**
     * @brief   Get the element at the given index.
     * 
     * @note    No bounds checking is performed.
     * 
     * @param   index
     *          The (zero-based) index of the element to return.
     */
    T &operator[](size_t index) { return data[index]; }

    /**
     * @brief   Get the element at the given index.
     * 
     * @note    No bounds checking is performed.
     * 
     * @param   index
     *          The (zero-based) index of the element to return.
     */
    const T &operator[](size_t index) const { return data[index]; }

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

    /**
     * @brief   Get a copy of a slice of the array.
     * 
     * @tparam  Start 
     *          The index of the first element to copy.
     * @tparam  End
     *          The index of the first element not to copy, one index beyond the
     *          end of the new array
     * 
     * For example:
     * ```
     * Array<int, 5> a = {1, 2, 3, 4, 5};
     * Array<int, 3> b = a.slice<1, 4>();
     * // b == {2, 3, 4}
     * ```
     */
    template <size_t Start, size_t End = N>
    USE_CONSTEXPR_ARRAY_HELPERS Array<T, End - Start> slice() const {
        Array<T, End - Start> result = {};
        for (size_t i = Start, j = 0; i < End; ++i, ++j)
            result[j] = (*this)[i];
        return result;
    }
};
