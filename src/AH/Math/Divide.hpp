#pragma once

#include <AH/STL/cstdint>
#include <AH/STL/type_traits>
#include <AH/Settings/NamespaceSettings.hpp>
#include <stddef.h>

BEGIN_AH_NAMESPACE

/// Divide by N using the default division operator, without explicit rounding
/// This should be used for floating point types. For integers, prefer using
/// @ref round_div_unsigned_int and @ref round_div_signed_int.
template <uint8_t N, class T>
struct round_div_default {
    static T div(T val) { return val / N; }
};

/// Divide an unsigned integer by N, rounding the result.
template <uint8_t N, class T>
struct round_div_unsigned_int {
    static T div(T val) {
        return (val + (N / 2)) / N;
        static_assert(std::is_unsigned<T>::value && std::is_integral<T>::value,
                      "This function is only valid for unsigned integers");
    }
};

/// Divide a signed integer by N, rounding the result.
template <uint8_t N, class T>
struct round_div_signed_int {
    static T div(T val) {
        T offset = val >= 0 ? (N / 2) : (-N / 2);
        return (val + offset) / N;
    }
};

/// Select the right rounding division operator, depending on whether T is a
/// signed or unsigned integer.
template <uint8_t N, class T>
struct round_div_int
    : std::conditional<std::is_signed<T>::value, round_div_signed_int<N, T>,
                       round_div_unsigned_int<N, T>>::type {};

/// Select the right rounding division operator, depending on whether T is an
/// integer or not.
template <uint8_t N, class T>
struct round_div_helper
    : std::conditional<std::is_integral<T>::value, round_div_int<N, T>,
                       round_div_default<N, T>>::type {};

/// Divide a number by N and round the result. Uses different specializations
/// for integers to implement efficient rounding.
template <size_t N, class T>
T round_div(T val) {
    return round_div_helper<N, T>::div(val);
}

END_AH_NAMESPACE
