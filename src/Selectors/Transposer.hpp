#pragma once

#include <Banks/Bank.h>
#include <Control_Surface/Control_Surface_Class.h>
#include <Def/Def.hpp>

template <int8_t MINIMUM_SEMITONES, int8_t MAXIMUM_SEMITONES>
class Transposer : public Bank<MAXIMUM_SEMITONES - MINIMUM_SEMITONES + 1> {
  public:
    Transposer(int8_t step = 1) : Bank<N>(step, -MINIMUM_SEMITONES) {
        static_assert(MINIMUM_SEMITONES <= 0,
                      "Error: the minimum transposition must be negative");
        static_assert(MAXIMUM_SEMITONES >= 0,
                      "Error: the maximum transposition must be positive");
    }
    /**
     * @brief Get the current transposition offset.
     * 
     * @todo    The return type is a uint8_t, but it can be negative.
     *          This is not an issue, because the result will be the same, 
     *          since the addition will just overflow and truncate, but
     *          it's not really clean.
     * 
     * @return uint8_t 
     */
    uint8_t getSelection() const override {
        return Bank<MAXIMUM_SEMITONES - MINIMUM_SEMITONES + 1>::getSelection() +
               MINIMUM_SEMITONES;
    }

    static constexpr setting_t N = MAXIMUM_SEMITONES - MINIMUM_SEMITONES + 1;
};