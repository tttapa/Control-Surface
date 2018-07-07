#pragma once

#include "../Control_Surface/Control_Surface_Class.h"
#include "Abstract/MIDIButtonLatching.hpp"

/**
 * @brief   A class for latching buttons and switches that send MIDI Note 
 *          events.
 * 
 * The switch is debounced.
 * 
 * @see     Button
 */
class NoteButtonLatching : public MIDIButtonLatching {
  public:
    /**
     * @brief   Construct a new NoteButtonLatching.
     * 
     * @param   pin
     *          The digital input pin with the switch connected.  
     *          The internal pull-up resistor will be enabled.
     * @param   address
     *          The MIDI Note number. [0, 127]
     * @param   channel
     *          The MIDI Channel. [1, 16]
     * @param   velocity
     *          The MIDI velocity value. [1, 127]
     */
    NoteButtonLatching(pin_t pin, uint8_t address, uint8_t channel,
                       uint8_t velocity = 127)
        : MIDIButtonLatching(pin), address(address), channel(channel),
          velocity(velocity) {}

  private:
    void sendOn() const override {
        Control_Surface.MIDI()->send(
            NOTE_ON, channel + channelOffset * tracksPerBank,
            address + addressOffset * tracksPerBank, velocity);
    }
    void sendOff() const override {
        Control_Surface.MIDI()->send(
            NOTE_OFF, channel + channelOffset * tracksPerBank,
            address + addressOffset * tracksPerBank, velocity);
    }

    const uint8_t address;
    const uint8_t channel;
    uint8_t velocity;
};