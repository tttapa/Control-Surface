#pragma once

#include <Audio.h>
#include <Helpers/EMA.hpp>
#include <MIDI_Inputs/MCU/VU.hpp>

#include "MovingCoilBallistics.hpp"

/**
 * @brief   A VU meter that reads from an Audio stream using the 
 *          AudioAnalyzePeak class.
 * 
 * @ingroup Audio
 */
class AudioVU : public IVU {
  public:
    /** 
     * @brief   Create a new AudioVU object.
     * 
     * @param   level
     *          The Teensy Audio peak analyzer object.  
     *          Note that it is kept by reference, so it must outlive the 
     *          AudioVU instance.
     * @param   gain
     *          A multiplier to calibrate the VU meter.
     * @param   max
     *          The max output, or the length of the output scale:
     *          `getValue` will output a number in [0, max].
     */
    AudioVU(AudioAnalyzePeak &level, float gain = 1.0, uint8_t max = 100)
        : IVU(max), level(level), gain(gain) {}

    /** 
     * @brief   Create a new AudioVU object.
     * 
     * @param   level
     *          The Teensy Audio peak analyzer object.  
     *          Note that it is kept by reference, so it must outlive the 
     *          AudioVU instance.
     * @param   ballistics
     *          The dynamic model to use for the VU meter.
     * @param   gain
     *          A multiplier to calibrate the VU meter.
     * @param   max
     *          The max output, or the length of the output scale:
     *          `getValue` will output a number in [0, max].
     */
    AudioVU(AudioAnalyzePeak &level, MovingCoilBallistics ballistics,
            float gain = 1.0, uint8_t max = 100)
        : IVU(max), ballistics(ballistics), level(level), gain(gain) {}

    /** 
     * @brief   Get the value of the VU meter.
     * 
     * @return  A value in [0, max]
     */
    uint8_t getValue() const override {
        if (!level.available())
            return 0;
        float peakLevel = level.read();
        // TODO: should I round here?
        uint8_t value = ballistics(peakLevel) * gain * (max + 1);
        if (value > max)
            value = max;
        return value;
    }

    /** @note   This function will always return false for an AudioVU. */
    bool getOverload() const override { return false; } // TODO
    /** 
     * @brief   Set the gain for the VU meter.
     * 
     * @param   gain
     *          The new gain.
     */
    void setGain(float gain) { this->gain = gain; }

  private:
    mutable MovingCoilBallistics ballistics =
        MovingCoilBallistics::responsiveVU();
    AudioAnalyzePeak &level;
    float gain;
};