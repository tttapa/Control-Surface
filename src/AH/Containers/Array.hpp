/* ✔ */

#pragma once

#include <AH/Error/Error.hpp>
#include <AH/STL/type_traits> // conditional
#include <stddef.h>           // size_t

BEGIN_AH_NAMESPACE

/// @addtogroup Containers
/// @{

template <class T, size_t N, size_t Start, size_t End, bool Const>
class ArraySlice;

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

    /**
     * @brief   Get a view on a slice of the Array.
     * 
     * Doesn't copy the contents of the array, it's just a reference to the 
     * original array.
     * 
     * @tparam  Start
     *          The start index of the slice.
     * @tparam  End
     *          The end index of the slice (non inclusive).
     */
    template <size_t Start = 0, size_t End = N>
    ArraySlice<T, N, Start, End, false> slice();

    /**
     * @brief   Get a read-only view on a slice of the Array.
     * @copydetails     slice()
     */
    template <size_t Start = 0, size_t End = N>
    ArraySlice<T, N, Start, End, true> slice() const;

    /**
     * @brief   Get a read-only view on a slice of the Array.
     * @copydetails     slice()
     */
    template <size_t Start = 0, size_t End = N>
    ArraySlice<T, N, Start, End, true> cslice() const {
        const Array<T, N> &This = *this;
        return This.slice();
    }
};

/**
 * @brief   Class for a view on a slice of an array.
 * 
 * Doesn't copy the contents of the array, it's just a reference to the original
 * array.
 * 
 * @tparam  T
 *          The type of elements of the Array.
 * @tparam  N 
 *          The size of the Array.
 * @tparam  Start
 *          The start index of the slice.
 * @tparam  End
 *          The end index of the slice (non inclusive).
 * @tparam  Const
 *          Whether to save a read-only or mutable reference to the Array.
 */
template <class T, size_t N, size_t Start = 0, size_t End = N,
          bool Const = true>
class ArraySlice {
    using ArrayRefType = typename std::conditional<Const, const Array<T, N> &,
                                                   Array<T, N> &>::type;
    using ElementRefType =
        typename std::conditional<Const, const T &, T &>::type;
    using ElementPtrType =
        typename std::conditional<Const, const T *, T *>::type;

  public:
    /// Constructor
    ArraySlice(ArrayRefType array) : array{array} {}

    /// Implicit conversion from slice to new array (creates a copy).
    operator Array<T, End - Start>() const { return asArray(); }

    Array<T, End - Start> asArray() const {
        Array<T, End - Start> slice = {};
        for (size_t i = 0; i < End - Start; ++i)
            slice[i] = array[Start + i];
        return slice;
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
    ElementRefType operator[](size_t index) const {
        return array[Start + index];
    }

    ElementPtrType begin() const { return array.begin() + Start; }
    ElementPtrType end() const { return array.begin() + End; }

    template <size_t NewStart, size_t NewEnd>
    ArraySlice<T, N, Start + NewStart, Start + NewEnd, Const> slice() const;

  private:
    ArrayRefType &array;
};

template <class T, size_t N>
template <size_t Start, size_t End>
inline ArraySlice<T, N, Start, End, false> Array<T, N>::slice() {
    return ArraySlice<T, N, Start, End, false>{*this};
}

template <class T, size_t N>
template <size_t Start, size_t End>
inline ArraySlice<T, N, Start, End, true> Array<T, N>::slice() const {
    return ArraySlice<T, N, Start, End, true>{*this};
}

template <class T, size_t N, size_t Start, size_t End, bool Const>
template <size_t NewStart, size_t NewEnd>
ArraySlice<T, N, Start + NewStart, Start + NewEnd, Const>
ArraySlice<T, N, Start, End, Const>::slice() const {
    static_assert(NewStart < NewEnd, "");
    static_assert(NewEnd < End - Start, "");
    return array;
}

// Equality ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

/// Slice == Slice
template <class T1, class T2, size_t N1, size_t N2, size_t Start1,
          size_t Start2, size_t End1, size_t End2, bool Const1, bool Const2>
bool operator==(ArraySlice<T1, N1, Start1, End1, Const1> a,
                ArraySlice<T2, N2, Start2, End2, Const2> b) {
    static_assert(End1 - Start1 == End2 - Start2, "Error: sizes do not match");
    for (size_t i = 0; i < End1 - Start1; ++i)
        if (a[i] != b[i])
            return false;
    return true;
}

/// Array == Slice
template <class T1, class T2, size_t N1, size_t N2, size_t Start2, size_t End2,
          bool Const2>
bool operator==(const Array<T1, N1> &a,
                ArraySlice<T2, N2, Start2, End2, Const2> b) {
    return a.slice() == b;
}

/// Slice == Array
template <class T1, class T2, size_t N1, size_t N2, size_t Start1, size_t End1,
          bool Const1>
bool operator==(ArraySlice<T2, N2, Start1, End1, Const1> a,
                const Array<T1, N1> &b) {
    return a == b.slice();
}

// Addition ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

/// Slice + Slice
template <class T1, class T2, size_t N1, size_t N2, size_t Start1,
          size_t Start2, size_t End1, size_t End2, bool Const1, bool Const2>
Array<decltype(T1{} + T2{}), End1 - Start1>
operator+(ArraySlice<T1, N1, Start1, End1, Const1> a,
          ArraySlice<T2, N2, Start2, End2, Const2> b) {
    static_assert(End1 - Start1 == End2 - Start2, "Error: sizes do not match");
    Array<decltype(T1{} + T2{}), End1 - Start1> result = {};
    for (size_t i = 0; i < End1 - Start1; ++i)
        result[i] = a[i] + b[i];
    return result;
}

/// Array + Array
template <class T1, class T2, size_t N1, size_t N2>
Array<decltype(T1{} + T2{}), N1> operator+(const Array<T1, N1> &a,
                                           const Array<T2, N2> &b) {
    return a.slice() + b.slice();
}

/// Slice += Slice
template <class T1, class T2, size_t N1, size_t N2, size_t Start1,
          size_t Start2, size_t End1, size_t End2, bool Const1, bool Const2>
const ArraySlice<T1, N1, Start1, End1, Const1> &
operator+=(const ArraySlice<T1, N1, Start1, End1, Const1> &a,
           const ArraySlice<T2, N2, Start2, End2, Const2> &b) {
    static_assert(End1 - Start1 == End2 - Start2, "Error: sizes do not match");
    for (size_t i = 0; i < End1 - Start1; ++i)
        a[i] += b[i];
    return a;
}

/// Array += Array
template <class T1, class T2, size_t N1, size_t N2>
Array<T1, N1> &operator+=(Array<T1, N1> &a, const Array<T2, N2> &b) {
    a.slice() += b.slice();
    return a;
}

// Subtraction :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

/// Slice - Slice
template <class T1, class T2, size_t N1, size_t N2, size_t Start1,
          size_t Start2, size_t End1, size_t End2, bool Const1, bool Const2>
Array<decltype(T1{} - T2{}), End1 - Start1>
operator-(ArraySlice<T1, N1, Start1, End1, Const1> a,
          ArraySlice<T2, N2, Start2, End2, Const2> b) {
    static_assert(End1 - Start1 == End2 - Start2, "Error: sizes do not match");
    Array<decltype(T1{} - T2{}), End1 - Start1> result = {};
    for (size_t i = 0; i < End1 - Start1; ++i)
        result[i] = a[i] - b[i];
    return result;
}

/// Array - Array
template <class T1, class T2, size_t N1, size_t N2>
Array<decltype(T1{} - T2{}), N1> operator-(const Array<T1, N1> &a,
                                           const Array<T2, N2> &b) {
    return a.slice() - b.slice();
}

/// Slice -= Slice
template <class T1, class T2, size_t N1, size_t N2, size_t Start1,
          size_t Start2, size_t End1, size_t End2, bool Const1, bool Const2>
const ArraySlice<T1, N1, Start1, End1, Const1> &
operator-=(const ArraySlice<T1, N1, Start1, End1, Const1> &a,
           const ArraySlice<T2, N2, Start2, End2, Const2> &b) {
    static_assert(End1 - Start1 == End2 - Start2, "Error: sizes do not match");
    for (size_t i = 0; i < End1 - Start1; ++i)
        a[i] -= b[i];
    return a;
}

/// Array -= Array
template <class T1, class T2, size_t N1, size_t N2>
Array<T1, N1> &operator-=(Array<T1, N1> &a, const Array<T2, N2> &b) {
    a.slice() -= b.slice();
    return a;
}

// Scalar Multiplication :::::::::::::::::::::::::::::::::::::::::::::::::::::::

/// Slice * Scalar
template <class T1, class T2, size_t N1, size_t Start1, size_t End1,
          bool Const1>
Array<decltype(T1{} * T2{}), End1 - Start1>
operator*(ArraySlice<T1, N1, Start1, End1, Const1> a, T2 b) {
    Array<decltype(T1{} * T2{}), End1 - Start1> result = {};
    for (size_t i = 0; i < End1 - Start1; ++i)
        result[i] = a[i] * b;
    return result;
}

/// Array * Scalar
template <class T1, class T2, size_t N1>
Array<decltype(T1{} * T2{}), N1> operator*(const Array<T1, N1> &a, T2 b) {
    return a.slice() * b;
}

/// Scalar * Slice
template <class T1, class T2, size_t N2, size_t Start2, size_t End2,
          bool Const2>
Array<decltype(T1{} * T2{}), End2 - Start2>
operator*(T1 a, ArraySlice<T2, N2, Start2, End2, Const2> b) {
    Array<decltype(T1{} * T2{}), End2 - Start2> result = {};
    for (size_t i = 0; i < End2 - Start2; ++i)
        result[i] = a * b[i];
    return result;
}

/// Scalar * Array
template <class T1, class T2, size_t N2>
Array<decltype(T1{} * T2{}), N2> operator*(T1 a, const Array<T2, N2> &b) {
    return a * b.slice();
}

/// Slice *= Scalar
template <class T1, class T2, size_t N1, size_t Start1, size_t End1,
          bool Const1>
const ArraySlice<T1, N1, Start1, End1, Const1> &
operator*=(const ArraySlice<T1, N1, Start1, End1, Const1> &a, T2 b) {
    for (size_t i = 0; i < End1 - Start1; ++i)
        a[i] *= b;
    return a;
}

/// Array *= Scalar
template <class T1, class T2, size_t N1>
Array<T1, N1> &operator*=(Array<T1, N1> &a, T2 b) {
    a.slice() *= b;
    return a;
}

// Scalar Division :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

/// Slice / Scalar
template <class T1, class T2, size_t N1, size_t Start1, size_t End1,
          bool Const1>
Array<decltype(T1{} / T2{}), End1 - Start1>
operator/(ArraySlice<T1, N1, Start1, End1, Const1> a, T2 b) {
    Array<decltype(T1{} / T2{}), End1 - Start1> result = {};
    for (size_t i = 0; i < End1 - Start1; ++i)
        result[i] = a[i] / b;
    return result;
}

/// Array / Scalar
template <class T1, class T2, size_t N1>
Array<decltype(T1{} / T2{}), N1> operator/(const Array<T1, N1> &a, T2 b) {
    return a.slice() / b;
}

/// Slice /= Scalar
template <class T1, class T2, size_t N1, size_t Start1, size_t End1,
          bool Const1>
const ArraySlice<T1, N1, Start1, End1, Const1> &
operator/=(const ArraySlice<T1, N1, Start1, End1, Const1> &a, T2 b) {
    for (size_t i = 0; i < End1 - Start1; ++i)
        a[i] /= b;
    return a;
}

/// Array /= Scalar
template <class T1, class T2, size_t N1>
Array<T1, N1> &operator/=(Array<T1, N1> &a, T2 b) {
    a.slice() /= b;
    return a;
}

// Type aliases ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

/// An easy alias for two-dimensional Arrays.
template <class T, size_t nb_rows, size_t nb_cols>
using Array2D = Array<Array<T, nb_cols>, nb_rows>;

/// @}

END_AH_NAMESPACE