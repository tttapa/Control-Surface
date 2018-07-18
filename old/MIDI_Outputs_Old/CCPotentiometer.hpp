#pragma once

#include "../Control_Surface/Control_Surface_Class.h"
#include "Abstract/MIDIFilteredAnalog.hpp"

/**
 * @brief   A class for potentiometers and faders that send 7-bit MIDI
 *          Controller change events.
 * 
 * The analog input is filtered and hysteresis is applied.
 * 
 * @see     FilteredAnalog
 */
class CCPotentiometer : public MIDIFilteredAnalog<7> {
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
        : MIDIFilteredAnalog<7>(analogPin), address(address), channel(channel) {}

  private:
    void send(uint16_t value) const final override {
        Control_Surface.MIDI().send(
            CONTROL_CHANGE, channel + channelOffset * tracksPerBank,
            address + addressOffset * tracksPerBank, value);
    }

    const uint8_t address;
    const uint8_t channel;
};