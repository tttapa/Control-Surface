#pragma once

#include <Audio/AudioVU.hpp>
#include <Def/Def.hpp>
#include <Hardware/LEDs/VULEDs.hpp>

/**
 * @brief   A VU meter that reads from an Audio stream using the 
 *          AudioAnalyzePeak class, and displays it on an LED bar graph.
 * 
 * @tparam  N 
 *          The number of LEDs.
 * 
 * @ingroup Audio
 */
template <uint8_t N>
class AudioVULEDs : public AudioVU, public Updatable<AudioVU> {
  public:
    AudioVULEDs(const DotBarDisplayLEDs<N> &vuleds, AudioAnalyzePeak &level,
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

    void setMode(DotBarMode mode) { vuleds.setMode(mode); }
    void dotMode() { vuleds.dotMode(); }
    void barMode() { vuleds.barMode(); }

  private:
    DotBarDisplayLEDs<N> vuleds;
    uint8_t previousValue = 0;
};