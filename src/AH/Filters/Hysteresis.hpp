#pragma once

#include <AH/Settings/NamespaceSettings.hpp>

#include <stdint.h>

BEGIN_AH_NAMESPACE

/// @addtogroup    AH_Filters
/// @{

/**
 * @brief   A class for applying hysteresis to a given input.
 *
 * This reduces the noise by decreasing the resolution, and it prevents flipping
 * back and forth between two values.
 *
 * <b>An example for `BITS` = 7 and an input from 0 to 1023</b>
 * ```
 *    7                                                     ┌───◄───┬───
 * o  6                                             ┌───◄───┼───►───┘
 * u  5                                     ┌───◄───┼───►───┘
 * t  4                             ┌───◄───┼───►───┘
 * p  3                     ┌───◄───┼───►───┘
 * u  2             ┌───◄───┼───►───┘
 * t  1     ┌───◄───┼───►───┘
 *    0 ────┴───►───┘
 *      0      128     256     384     512     640     768     896    1023
 *                                  i n p u t
 * ```
 *
 * @tparam  BITS
 *          The number of bits to decrease in resolution.
 *          Increasing this number will result in a decrease in fluctuations.
 */
template <uint8_t Bits, class T_in = uint16_t, class T_out = uint8_t>
class Hysteresis {
  public:
    /**
     * @brief   Update the hysteresis output with a new input value.
     *
     * @param   inputLevel
     *          The input to calculate the output level from.
     * @retval  true
     *          The output level has changed.
     * @retval  false
     *          The output level is still the same.
     */
    bool update(T_in inputLevel) {
        T_in prevLevelFull = (T_in(prevLevel) << Bits) | offset;
        T_in lowerbound = prevLevel > 0 ? prevLevelFull - margin : 0;
        T_in upperbound = prevLevel < max_out ? prevLevelFull + margin : max_in;
        if (inputLevel < lowerbound || inputLevel > upperbound) {
            setValue(inputLevel);
            return true;
        }
        return false;
    }

    /**
     * @brief   Get the current output level.
     *
     * @return  The output level.
     */
    T_out getValue() const { return prevLevel; }

    /** 
     * @brief   Forcefully update the internal state to the given level.
     */
    void setValue(T_in inputLevel) { prevLevel = inputLevel >> Bits; }

  private:
    T_out prevLevel = 0;
    constexpr static T_in margin = (1ul << Bits) - 1ul;
    constexpr static T_in offset = Bits >= 1 ? 1ul << (Bits - 1) : 0;
    constexpr static T_in max_in = static_cast<T_in>(-1);
    constexpr static T_out max_out = static_cast<T_out>(max_in >> Bits);
    static_assert(max_in > 0, "Error: only unsigned types are supported");
};

template <class T_in, class T_out>
class Hysteresis<0, T_in, T_out> {
  public:
    bool update(T_in inputLevel) {
        bool changed = inputLevel != prevLevel;
        prevLevel = inputLevel;
        return changed;
    }

    T_out getValue() const { return prevLevel; }
    void setValue(T_in inputLevel) { prevLevel = inputLevel; }

  private:
    T_in prevLevel = 0;
};

/// @}

END_AH_NAMESPACE
