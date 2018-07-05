#pragma once

#include "../Hardware/FilteredAnalog.h"
#include "Abstract/CCOut.h"

/**
 * @brief   A class for potentiometers and faders that send 7-bit MIDI
 *          Controller change events.
 * 
 * The analog input is filtered and hysteresis is applied.
 * 
 * @see     FilteredAnalog
 */
class CCPotentiometer : public CCOut {
  public:
    /**
     * @brief   Construct a new CCPotentiometer.
     * 
     * @param   analogPin
     *          The analog input pin with the wiper of the potentiometer
     *          connected.  
     * @param   address
     *          The MIDI Controller number. [0, 119]
     * @param   channel
     *          The MIDI Channel. [1, 16]
     */
    CCPotentiometer(pin_t analogPin, uint8_t address, uint8_t channel)
        : CCOut(address, channel), filteredAnalog{analogPin} {}

  private:
    void refresh() {
        if (filteredAnalog.update())
            send(filteredAnalog.getValue());
    }

    FilteredAnalog<7> filteredAnalog; // 7 bits of precision
};