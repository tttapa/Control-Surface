#pragma once

#include "../Control_Surface/Control_Surface_Class.h"
#include "Abstract/MIDIFilteredAnalog.hpp"

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
class PBPotentiometer : public MIDIFilteredAnalog<8> {
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
        : MIDIFilteredAnalog(analogPin), channel(channel) {}

  private:
    void send(uint16_t value) const final override {
        value = (value << (14-8)) | (value >> (8-(14-8))); // Convert from 8-bit to 14-bit number
        Control_Surface.MIDI()->send(PITCH_BEND,
                                     channel + channelOffset * tracksPerBank,
                                     value & 0x7F, value >> 7);
    }

    const uint8_t channel;
};