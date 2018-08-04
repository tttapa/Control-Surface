#pragma once

#include "Array.hpp"

// https://en.cppreference.com/w/cpp/algorithm/generate
template <class ForwardIt, class Generator>
constexpr void generate(ForwardIt first, ForwardIt last, Generator g) {
    while (first != last) {
        *first++ = g();
    }
}

template <class T>
class IncrementalArrayGenerator {
  public:
    constexpr IncrementalArrayGenerator(T start = 0, T increment = 1)
        : value(start), increment(increment) {}
    constexpr T operator()() {
        T temp = value;
        value += increment;
        return temp;
    }

  private:
    T value;
    const T increment;
};

template <class T, size_t N>
constexpr Array<T, N> generateIncrementalArray(T start = 0, T increment = 1) {
    Array<T, N> array = {};
    IncrementalArrayGenerator<T> g(start, increment);
    generate(array.begin(), array.end(), g);
    return array;
}