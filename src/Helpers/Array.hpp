/* ✔ */

#pragma once

#include <Arduino.h>
#include <stddef.h>

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
    T &operator[](size_t index) {
        if (index >= N) { // TODO
            Serial.println(__PRETTY_FUNCTION__);
            Serial.print(F("Index out of bounds: "));
            Serial.println(index);
            index = N - 1;
#ifdef ARDUINO
            delay(5000);
#endif
        }
        return data[index];
    }

    /**
     * @brief   Get the element at the given index.
     * 
     * @note    No bounds checking is performed.
     * 
     * @param   index
     *          The (zero-based) index of the element to return.
     */
    const T &operator[](size_t index) const {
        if (index >= N) { // TODO
            Serial.println(__PRETTY_FUNCTION__);
            Serial.print(F("Index out of bounds: "));
            Serial.println(index);
            index = N - 1;
#ifdef ARDUINO
            delay(5000);
#endif
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