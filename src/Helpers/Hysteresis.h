#pragma once

#include <stdint.h>

/**
 * @brief   A class for applying hysteresis to a given input.
 *
 *          This reduces the noise by decreasing the resolution, and it prevents
 *          flipping back and forth between two values.
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
 *                                i n p u t
 * ```
 *
 * @tparam  BITS
 *          The number of bits to decrease in resolution.
 *          Increasing this number will result in a decrease in fluctuations.
 */
template <uint8_t BITS>
class Hysteresis {
  public:
    /**
     * @brief     Get the output level for a given input.
     *
     * @param     inputLevel
     *            The input level to calculate the output level from.
     * @return    The output level.
     */
    uint16_t getOutputLevel(uint16_t inputLevel);

  private:
    uint8_t previousLevel = 0;
    constexpr static uint8_t margin = (1 << BITS) - 1;
    constexpr static uint8_t offset = 1 << (BITS - 1);
};

// ----------------------------- Implementation ----------------------------- //

template <uint8_t BITS>
inline uint16_t Hysteresis<BITS>::getOutputLevel(uint16_t inputLevel) {
    uint16_t previousLevelFull = ((uint16_t)previousLevel << BITS) | offset;
    uint16_t lowerbound = previousLevelFull;
    if (previousLevel > 0)
        lowerbound -= margin;
    uint16_t upperbound = previousLevelFull + margin;
    if (inputLevel < lowerbound || inputLevel > upperbound)
        previousLevel = inputLevel >> BITS;
    return previousLevel;
}