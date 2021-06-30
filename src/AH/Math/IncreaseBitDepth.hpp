#pragma once

#include <AH/Settings/Warnings.hpp>
AH_DIAGNOSTIC_WERROR() // Enable errors on warnings

#include <AH/STL/type_traits> // enable_if
#include <limits.h>           // CHAR_BIT
#include <stddef.h>           // size_t

BEGIN_AH_NAMESPACE

template <size_t Bits_out, size_t Bits_in, class T_out, class T_in>
std::enable_if_t<(Bits_out <= 2 * Bits_in), T_out>
increaseBitDepthImpl(T_in in) {
    constexpr size_t leftShift = Bits_out - Bits_in;
    constexpr size_t rightShift = Bits_in - leftShift;
    return (T_out(in) << leftShift) | (in >> rightShift);
}

template <size_t Bits_out, size_t Bits_in, class T_out, class T_in>
std::enable_if_t<(Bits_out > 2 * Bits_in), T_out>
increaseBitDepthImpl(T_in in) {
    constexpr size_t leftShift = Bits_out - Bits_in;
    return (T_out(in) << leftShift) |
           increaseBitDepthImpl<leftShift, Bits_in, T_out>(in);
}

/// @addtogroup    AH_Math
/// @{

/**
 * @brief   Increase the bit depth of the given value from `Bits_in` bits wide
 *          to `Bits_out` bits wide, (approximately) evenly distributing the 
 *          error across the entire range, such that the error for each element
 *          is between -1 and +1.
 * 
 * For example, converting 3-bit numbers to 7-bit numbers would result in the
 * following:
 * 
 * | in (dec) | in (bin) | out (bin) | out (dec) | exact  | error |
 * |:--------:|:--------:|:---------:|:---------:|:------:|:-----:|
 * | 0        | 000      | 000'0000  | 0         | 0.00   | +0.00 |
 * | 1        | 001      | 001'0010  | 18        | 18.14  | +0.14 |
 * | 2        | 010      | 010'0100  | 36        | 36.29  | +0.29 |
 * | 3        | 011      | 011'0110  | 54        | 54.43  | +0.43 |
 * | 4        | 100      | 100'1001  | 73        | 72.57  | -0.43 |
 * | 5        | 101      | 101'1011  | 91        | 90.71  | -0.29 |
 * | 6        | 110      | 110'1101  | 109       | 108.86 | -0.14 |
 * | 7        | 111      | 111'1111  | 127       | 127.00 | +0.00 |
 * 
 * @tparam  Bits_out 
 *          The number of bits of the output range.
 * @tparam  Bits_in 
 *          The number of bits of the input range.
 * @tparam  T_out 
 *          The type of the output (return type).
 * @tparam  T_in 
 *          The type of the input.
 * @param   in 
 *          The value to scale up.
 * @return  The scaled up value.
 */
template <size_t Bits_out, size_t Bits_in, class T_out, class T_in>
T_out increaseBitDepth(T_in in) {
    static_assert(Bits_in <= sizeof(T_in) * CHAR_BIT,
                  "Error: Bits_in > bits(T_in)");
    static_assert(Bits_out <= sizeof(T_out) * CHAR_BIT,
                  "Error: Bits_out > bits(T_out)");
    return increaseBitDepthImpl<Bits_out, Bits_in, T_out>(in);
}

/// @}

END_AH_NAMESPACE

AH_DIAGNOSTIC_POP()
