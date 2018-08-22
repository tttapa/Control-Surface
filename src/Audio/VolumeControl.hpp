#pragma once

#include <Arduino.h>
#include <Helpers/Updatable.hpp>
#include <Hardware/FilteredAnalog.h>
#include <Audio.h>

/** 
 * @brief   A class for controlling the volume of AudioMixer4 objects using a 
 *          potentiometer.
 * 
 * @tparam  N
 *          The number of mixers.
 * 
 * @ingroup Audio
 */
template <uint8_t N>
class VolumeControl : public Updatable {
  public:
    VolumeControl(const Array<AudioMixer4*, N> &mixers, pin_t analogPin,
                  float maxGain = 1.0) 
      : mixers(mixers), filteredAnalog(analogPin), maxGain(maxGain) {}
    void update() override {
      if (filteredAnalog.update()) {
        float gain = filteredAnalog.getValue() * maxGain / 127.0;
        for (AudioMixer4 *mixer : mixers)
          for (uint8_t ch = 0; ch < 4; ch ++) 
            mixer->gain(ch, gain);
      }
    }
    void begin() override {}
    
  private:
    Array<AudioMixer4*, N> mixers;
    FilteredAnalog<7> filteredAnalog;
    const float maxGain;
};