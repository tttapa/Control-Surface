/* ✔ */

#pragma once

#include "Array.hpp"

#if __cplusplus >= 201400L
#define USE_CONSTEXPR_ARRAY_HELPERS constexpr
#else
#define USE_CONSTEXPR_ARRAY_HELPERS
#endif

BEGIN_AH_NAMESPACE

/// https://en.cppreference.com/w/cpp/algorithm/generate
template <class ForwardIt, class Generator>
USE_CONSTEXPR_ARRAY_HELPERS void generate(ForwardIt first, ForwardIt last,
                                          Generator g) {
    while (first != last)
        *first++ = g();
}

/** 
 * @brief   Utility class that acts as a functor to return incremental values.
 * 
 * @tparam  T
 *          The type that will be returned by the functor, as well as the type
 *          of the initial value.
 * @tparam  V
 *          The type of the object that is added to the value on each call.
 */
template <class T, class V>
class Incrementor {
  public:
    USE_CONSTEXPR_ARRAY_HELPERS Incrementor(T start = 0, V increment = 1)
        : value(start), increment(increment) {}
    USE_CONSTEXPR_ARRAY_HELPERS T operator()() {
        T temp = value;
        value += increment;
        return temp;
    }

  private:
    T value;
    const V increment;
};

END_AH_NAMESPACE

/// @addtogroup Containers
/// @{

BEGIN_AH_NAMESPACE

/**
 * @brief   Generate an array using the given generator.
 * 
 * @tparam  T 
 *          The type of the elements in the array.
 * @tparam  N 
 *          The number of elements in the array.
 * @tparam  G
 *          The generator functor type.
 * 
 * @param   generator
 *          A functor that will be called to create each element.
 * 
 * @return  The generated array.
 */
template <class T, size_t N, class G>
USE_CONSTEXPR_ARRAY_HELPERS Array<T, N> generateArray(G generator) {
    Array<T, N> array{};
    generate(array.begin(), array.end(), generator);
    return array;
}

/**
 * @brief   Generate an array using the given generator.
 * 
 * @tparam  N 
 *          The number of elements in the array.
 * @tparam  G
 *          The generator functor type.
 * 
 * @param   generator
 *          A functor that will be called to create each element.
 * 
 * @return  The generated array.
 */
template <size_t N, class G>
USE_CONSTEXPR_ARRAY_HELPERS auto generateArray(G generator)
    -> Array<decltype(generator()), N> {
    Array<decltype(generator()), N> array{};
    generate(array.begin(), array.end(), generator);
    return array;
}

/**
 * @brief   Copy an Array to an Array of a different type.
 * 
 * @tparam  T 
 *          The type of the new array.
 * @tparam  N 
 *          The number of elements in the arrays.
 * @tparam  U 
 *          The type of the source array.
 * 
 * @param   src 
 *          The source array to be copied.
 */
template <class T, size_t N, class U>
USE_CONSTEXPR_ARRAY_HELPERS Array<T, N> copyAs(const Array<U, N> &src) {
    Array<T, N> dest{};
    for (size_t i = 0; i < N; ++i)
        dest[i] = src[i];
    return dest;
}

template <class T, size_t N, class... Args>
USE_CONSTEXPR_ARRAY_HELPERS Array<T, N> fillArray(Args... args) {
    return generateArray<N>([&]() { return T{args...}; });
}

/**
 * @brief   Generate an array where the first value is given, and the subsequent
 *          values are calculated as the previous value incremented with a given
 *          value:  
 *          @f$ x[0] = \mathrm{start} @f$  
 *          @f$ x[k+1] = x[k] + \mathrm{increment} @f$ .
 * 
 * For example:  
 * ```
 * auto x = generateIncrementalArray<unsigned int, 4>(2, 3);
 * ```
 * is equivalent to  
 * ```
 * Array<unsigned int, 4> x = {2, 5, 8, 11};
 * ```
 * 
 * @tparam  T 
 *          The type of the elements in the array.
 * @tparam  N 
 *          The number of elements in the array.
 * @tparam  U 
 *          The type of the initial value.
 * @tparam  V
 *          The type of the value that will be added to each subsequent element.
 * 
 * @param   start
 *          The first value in the array.
 * @param   increment
 *          The value to add to each subsequent element of the array.
 * 
 * @return  The generated array.
 */
template <class T, size_t N, class U, class V = U>
USE_CONSTEXPR_ARRAY_HELPERS Array<T, N>
generateIncrementalArray(U start = 0, V increment = 1) {
    Incrementor<U, V> g(start, increment);
    return generateArray<T, N>(g);
}

/**
 * @brief   Concatenate two arrays.
 * 
 * @tparam  T
 *          The type of the elements in the array. 
 * @tparam  M
 *          The number of elements in the first array.
 * @tparam  N 
 *          The number of elements in the second array.
 * @param   a 
 *          The first array.
 * @param   b 
 *          The second array.
 * @return  A new array containing the elements of both input arrays (in order). 
 */
template <class T, size_t M, size_t N>
USE_CONSTEXPR_ARRAY_HELPERS Array<T, M + N> cat(const Array<T, M> &a,
                                                const Array<T, N> &b) {
    Array<T, M + N> result = {};
    size_t r = 0;
    for (size_t i = 0; i < M; ++i, ++r)
        result[r] = a[i];
    for (size_t i = 0; i < N; ++i, ++r)
        result[r] = b[i];
    return result;
}

template <class T1, class T2, size_t N1, size_t N2, size_t Start1,
          size_t Start2, size_t End1, size_t End2, bool Const1, bool Const2>
USE_CONSTEXPR_ARRAY_HELPERS
    Array<decltype(T1{} * T2{}), End1 - Start1 + End2 - Start2 - 1>
    distribute(const ArraySlice<T1, N1, Start1, End1, Const1> &a,
               const ArraySlice<T2, N2, Start2, End2, Const2> &b) {
    Array<decltype(T1{} * T2{}), End1 - Start1 + End2 - Start2 - 1> result = {};
    for (size_t i = 0; i < End1 - Start1; ++i)
        for (size_t j = 0; j < End2 - Start2; ++j)
            result[i + j] += a[i] * b[j];
    return result;
}

template <class T1, class T2, size_t N1, size_t N2, size_t Start1, size_t End1,
          bool Const1>
USE_CONSTEXPR_ARRAY_HELPERS Array<decltype(T1{} * T2{}), End1 - Start1 + N2 - 1>
distribute(const ArraySlice<T1, N1, Start1, End1, Const1> &a,
           const Array<T2, N2> &b) {
    return distribute(a, b.slice());
}

template <class T1, class T2, size_t N1, size_t N2, size_t Start2, size_t End2,
          bool Const2>
USE_CONSTEXPR_ARRAY_HELPERS Array<decltype(T1{} * T2{}), N1 + End2 - Start2 - 1>
distribute(const Array<T1, N1> &a,
           const ArraySlice<T2, N2, Start2, End2, Const2> &b) {
    return distribute(a.slice(), b);
}

template <class T1, class T2, size_t N1, size_t N2>
USE_CONSTEXPR_ARRAY_HELPERS Array<decltype(T1{} * T2{}), N1 + N2 - 1>
distribute(const Array<T1, N1> &a, const Array<T2, N2> &b) {
    return distribute(a.slice(), b.slice());
}

END_AH_NAMESPACE

/// @}

#ifndef ARDUINO

#include <ostream>

template <class T, size_t N>
std::enable_if_t<std::is_arithmetic<T>::value, std::ostream &>
operator<<(std::ostream &os, const AH::Array<T, N> &a) {
    for (const T &el : a.template slice<0, N - 1>())
        os << el << ", ";
    os << a[N - 1];
    return os;
}

template <class T, size_t N, size_t Start, size_t End, bool Const>
std::enable_if_t<std::is_arithmetic<T>::value, std::ostream &>
operator<<(std::ostream &os, const AH::ArraySlice<T, N, Start, End, Const> &a) {
    for (const T &el : a.template slice<0, End - Start - 1>())
        os << el << ", ";
    os << a[End - Start - 1];
    return os;
}

#endif
