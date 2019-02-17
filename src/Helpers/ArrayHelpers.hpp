/* ✔ */

#pragma once

#include "Array.hpp"

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
USE_CONSTEXPR_ARRAY_HELPERS Array<T, N>
generateArray(G generator) {
    Array<T, N> array{};
    generate(array.begin(), array.end(), generator);
    return array;
}

/**
 * @brief   Generate an array where the first value is given, and the subsequent
 *          values are calculated as the previous value incremented with a given
 *          value:  
 *          @f$ x[0] = start @f$  
 *          @f$ x[k+1] = x[k] + increment @f$.
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
