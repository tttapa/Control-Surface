#pragma once

#include <Audio.h>
#include <Helpers/EMA.hpp>
#include <MIDI_Inputs/MCU/VU.hpp>

/**
 * @brief   A VU meter that reads from an Audio stream using the 
 *          AudioAnalyzePeak class.
 * 
 * @ingroup Audio
 */
class AudioVU : public IVU {
  public:
    AudioVU(AudioAnalyzePeak &level, float gain = 1.0, uint8_t max = 12)
        : level(level), gain(gain), max(max) {}
    uint8_t getValue() const override {
        if (!level.available())
            return 0;
        float peakLevel = level.read();
        uint8_t value = filter(peakLevel) * gain * (max + 1);
        if (value > max)
            value = max;
        return value;
    }
    bool getOverload() const override { return false; } // TODO
    void setGain(float gain) { this->gain = gain; }

  private:
    mutable EMA_f filter = {0.6}; // TODO: const-correctness?
    AudioAnalyzePeak &level;
    float gain;
    const uint8_t max;
};