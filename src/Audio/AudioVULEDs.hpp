/* ✔ */

#pragma once

#include <AH/Hardware/LEDs/DotBarDisplayLEDs.hpp>
#include <Audio/AudioVU.hpp>
#include <Def/Def.hpp>

BEGIN_CS_NAMESPACE

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
class AudioVULEDs : public AudioVU, public AH::Updatable<AudioVU> {
  public:
    /**
     * @brief   Create a new AudioVULEDs object.
     * 
     * @param   vuleds
     *          The LEDs to display the VU meter to.
     * @param   level
     *          The Teensy Audio peak analyzer object.  
     *          Note that it is kept by reference, so it must outlive the 
     *          AudioVU instance.
     * @param   gain
     *          A multiplier to calibrate the VU meter.
     */
    AudioVULEDs(const AH::DotBarDisplayLEDs<N> &vuleds, AudioAnalyzePeak &level,
                float gain = 1.0)
        : AudioVU(level, gain, N), vuleds(vuleds) {}

    /**
     * @brief   Initialize.
     */
    void begin() override { vuleds.begin(); }

    /**
     * @brief   Update the LEDs with the latest level.
     */
    void update() override {
        uint8_t newValue = this->getValue();
        if (newValue != previousValue) {
            vuleds.display(newValue);
            previousValue = newValue;
        }
    }

    /**
     * @brief   Set the mode to either dot or bar mode.
     * 
     * @param   mode 
     *          The mode.
     */
    void setMode(AH::DotBarMode mode) { vuleds.setMode(mode); }

    /**
     * @brief   Set the mode to dot mode.
     */
    void dotMode() { vuleds.dotMode(); }

    /**
     * @brief   Set the mode to bar mode.
     */
    void barMode() { vuleds.barMode(); }

  private:
    AH::DotBarDisplayLEDs<N> vuleds;
    uint8_t previousValue = 0;
};

END_CS_NAMESPACE