#pragma once

#include "Array.hpp"

#if __cplusplus >= 201400L
#define USE_CONSTEXPR_ARRAY_HELPERS constexpr
#else
#define USE_CONSTEXPR_ARRAY_HELPERS
#endif

// https://en.cppreference.com/w/cpp/algorithm/generate
template <class ForwardIt, class Generator>
USE_CONSTEXPR_ARRAY_HELPERS void generate(ForwardIt first, ForwardIt last,
                                          Generator g) {
    while (first != last)
        *first++ = g();
}

template <class T>
class Incrementor {
  public:
    USE_CONSTEXPR_ARRAY_HELPERS Incrementor(T start = 0, T increment = 1)
        : value(start), increment(increment) {}
    USE_CONSTEXPR_ARRAY_HELPERS T operator()() {
        T temp = value;
        value += increment;
        return temp;
    }

  private:
    T value;
    const T increment;
};

/**
 * @brief   Generate an array where the first value is given, and the subsequent
 *          values are calculated as the previous value incremented with a given
 *          value:  
 *          @f$ x[0] = start @f$  
 *          @f$ x[k+1] = x[k] + increment @f$.
 * 
 * For example:  
 * ```
 * auto x = generateArray<unsigned int, 4>(2, 3);
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
 * 
 * @param   start
 *          The first value in the array.
 * @param   increment
 *          The value to add to each subsequent element of the array.
 * 
 * @return  The generated array.
 */
template <class T, size_t N, class U>
USE_CONSTEXPR_ARRAY_HELPERS Array<T, N>
generateArray(U start = 0, U increment = 1) {
    Array<T, N> array = {};
    Incrementor<U> g(start, increment);
    generate(array.begin(), array.end(), g);
    return array;
}