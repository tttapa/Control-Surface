#pragma once

#include <Audio.h>
#include <Helpers/EMA.h>
#include <MIDI_Inputs/MCU/VU.hpp>

class AudioVU : public IVU {
  public:
    AudioVU(AudioAnalyzePeak &level, float gain = 1.0, uint8_t max = 12)
        : level(level), gain(gain), max(max) {}
    uint8_t getValue() override {
        if (!level.available())
            return 0;
        float peakLevel = level.read();
        uint8_t value = filter(peakLevel) * gain * max;
        if (value > max)
            value = max;
        return value;
    }
    bool getOverload() override { return false; } // TODO
    void setGain(float gain) { this->gain = gain; }

  private:
    EMA_f filter = 0.6;
    AudioAnalyzePeak &level;
    float gain;
    const uint8_t max;
};