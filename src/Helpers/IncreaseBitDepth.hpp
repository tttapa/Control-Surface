#pragma once

#include <limits.h>  // CHAR_BIT
#include <stddef.h>  // size_t

template <bool B, class T = void>
struct enable_if {};

template <class T>
struct enable_if<true, T> {
    typedef T type;
};

template <bool B, class T = void>
using enable_if_t = typename enable_if<B, T>::type;

template <size_t Bits_out, size_t Bits_in, class T_out, class T_in>
enable_if_t<(Bits_out <= 2 * Bits_in), T_out> increaseBitDepthImpl(T_in in);

template <size_t Bits_out, size_t Bits_in, class T_out, class T_in>
enable_if_t<(Bits_out > 2 * Bits_in), T_out> increaseBitDepthImpl(T_in in) {
    constexpr size_t leftShift = Bits_out - Bits_in;
    return (T_out(in) << leftShift) |
           increaseBitDepthImpl<leftShift, Bits_in, T_out>(in);
}

template <size_t Bits_out, size_t Bits_in, class T_out, class T_in>
enable_if_t<(Bits_out <= 2 * Bits_in), T_out> increaseBitDepthImpl(T_in in) {
    constexpr size_t leftShift  = Bits_out - Bits_in;
    constexpr size_t rightShift = Bits_in - leftShift;
    return (T_out(in) << leftShift) | (in >> rightShift);
}

template <size_t Bits_out, size_t Bits_in, class T_out, class T_in>
T_out increaseBitDepth(T_in in) {
    static_assert(Bits_in <= sizeof(T_in) * CHAR_BIT,
                  "Error: Bits_in > bits(T_in)");
    static_assert(Bits_out <= sizeof(T_out) * CHAR_BIT,
                  "Error: Bits_out > bits(T_out)");
    return increaseBitDepthImpl<Bits_out, Bits_in, T_out>(in);
}