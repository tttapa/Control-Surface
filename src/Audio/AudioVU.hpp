#pragma once

#include <Audio.h>
#include <MIDI_Inputs/InterfaceMIDIInputElements.hpp>

#include "MovingCoilBallistics.hpp"

BEGIN_CS_NAMESPACE

/**
 * @brief   A VU meter that reads from an Audio stream using the 
 *          Analyzer class.
 * 
 * @ingroup Audio
 */
class AudioVU : public Interfaces::MCU::IVU {
  public:
    /** 
     * @brief   Create a new AudioVU object.
     * 
     * @param   level
     *          The Teensy Audio peak or RMS analyzer object.  
     *          Note that it is kept by reference, so it must outlive the 
     *          AudioVU instance.
     * @param   gain
     *          A multiplier to calibrate the VU meter.
     * @param   max
     *          The max output, or the length of the output scale:
     *          `getValue` will output a number in [0, max].
     */
    template <class T>
    AudioVU(T &level, float gain = 1.0, uint8_t max = 255)
        : IVU(max, true), level(level), gain(gain) {}

    /** 
     * @brief   Create a new AudioVU object.
     * 
     * @param   level
     *          The Teensy Audio peak or RMS analyzer object.  
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
    template <class T>
    AudioVU(T &level, MovingCoilBallistics ballistics, float gain = 1.0,
            uint8_t max = 255)
        : IVU(max, true), ballistics(ballistics), level(level), gain(gain) {}

    /** 
     * @brief   Get the value of the VU meter.
     * 
     * @return  A value in [0, getMax()]
     */
    uint8_t getValue() override {
        uint16_t value = getFloatValue() * max;
        return value;
    }

    /** 
     * @brief   Get the value of the VU meter.
     * 
     * @return  A value in [0.0, 1.0]
     */
    float getFloatValue() override {
        if (!level.available())
            return 0;
        float peakLevel = level.read();
        float value = ballistics(peakLevel) * gain;
        if (value > 1.0f)
            value = 1.0f;
        else if (value < 0.0f)
            value = 0.0f;
        return value;
    }

    /** @note   This function will always return false for an AudioVU. */
    bool getOverload() override { return false; }

    /** 
     * @brief   Set the gain for the VU meter.
     * 
     * @param   gain
     *          The new gain.
     */
    void setGain(float gain) { this->gain = gain; }

  private:
    MovingCoilBallistics ballistics = MovingCoilBallistics::responsiveVU();

    /**
     * @brief   The `AudioAnalyzePeak` and `AudioAnalyzeRMS` classes don't 
     *          implement a common interface, so we have to use our own RTTI 
     *          wrapper to allow both classes to be used.
     */
    class Analyzer {
      private:
        enum { Peak, RMS } type;
        void *analyzer;

      public:
        Analyzer(AudioAnalyzePeak &analyzer) //
            : type(Peak), analyzer(&analyzer) {}
        Analyzer(AudioAnalyzeRMS &analyzer) //
            : type(RMS), analyzer(&analyzer) {}

        float read() const {
            switch (type) {
                case Peak:
                    return static_cast<AudioAnalyzePeak *>(analyzer)->read();
                case RMS:
                    return static_cast<AudioAnalyzeRMS *>(analyzer)->read();
                default: ERROR(F("Error: Invalid type!"), 0x1518); return 0;
            }
        }

        bool available() const {
            switch (type) {
                case Peak:
                    return static_cast<AudioAnalyzePeak *>(analyzer)
                        ->available();
                case RMS:
                    return static_cast<AudioAnalyzeRMS *>(analyzer)
                        ->available();
                default: ERROR(F("Error: Invalid type!"), 0x1519); return false;
            }
        }
    } level;

    float gain;
};

END_CS_NAMESPACE