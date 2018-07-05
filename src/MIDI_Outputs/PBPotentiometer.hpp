#pragma once

#include "../Hardware/FilteredAnalog.h"
#include "Abstract/PBOut.h"

/**
 * @brief   A class for potentiometers and faders that send 14-bit MIDI
 *          Pitch Bend events.
 * 
 * The analog input is filtered and hysteresis is applied.
 * 
 * @note    Even though the MIDI protocol allows for 14 bits of precision, 
 *          the ADC of most Arduinos is only 10 bits, and 2 bits are used
 *          for filtering and hysteresis.  
 *          This means that the effective resolution is only 8 bits.
 * 
 * @see     FilteredAnalog
 */
class PBPotentiometer : public PBOut {
  public:
    /**
     * @brief   Construct a new PBPotentiometer.
     * 
     * @param   analogPin
     *          The analog input pin with the wiper of the potentiometer
     *          connected.  
     * @param   channel
     *          The MIDI Channel. [1, 16]
     */
    PBPotentiometer(pin_t analogPin, uint8_t channel)
        : PBOut(channel), filteredAnalog{analogPin} {}

  private:
    void refresh() {
        if (filteredAnalog.update())
            send(filteredAnalog.getValue() << 6);
    }

    FilteredAnalog<8> filteredAnalog; // 8 bits of precision
};