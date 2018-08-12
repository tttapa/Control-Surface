#pragma once

#include <Audio/AudioVU.hpp>
#include <Hardware/LEDs/VULEDs.hpp>

template <uint8_t N>
class AudioVULEDs : public AudioVU, public Updatable {
  public:
    AudioVULEDs(const VULEDs<N> &vuleds, AudioAnalyzePeak &level,
                float gain = 1.0)
        : AudioVU(level, gain, N), vuleds(vuleds) {}

    void begin() override { vuleds.begin(); }
    void update() override {
        uint8_t newValue = this->getValue();
        if (newValue != previousValue) {
            vuleds.display(newValue);
            previousValue = newValue;
        }
    }

  private:
    const VULEDs<N> vuleds;
    uint8_t previousValue = 0;
};