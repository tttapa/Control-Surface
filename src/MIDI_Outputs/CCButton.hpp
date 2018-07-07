#pragma once

#include "../Control_Surface/Control_Surface_Class.h"
#include "Abstract/MIDIButton.hpp"

/**
 * @brief   A class for momentary push buttons that send MIDI Controller change
 *          events.
 * 
 * The button is debounced.
 * 
 * @see     Button
 */
class CCButton : public MIDIButton {
  public:
    /**
     * @brief   Construct a new CCButton.
     * 
     * @param   pin
     *          The digital input pin with the button connected.  
     *          The internal pull-up resistor will be enabled.
     * @param   address
     *          The MIDI Controller number. [0, 119]
     * @param   channel
     *          The MIDI Channel. [1, 16]
     * @param   offValue
     *          The Controller value to send when the button is released.
     *          [0,127]
     * @param   onValue
     *          The Controller value to send when the button is pressed.
     *          [0,127]
     */
    CCButton(pin_t pin, uint8_t address, uint8_t channel, uint8_t offValue = 0,
             uint8_t onValue = 127)
        : MIDIButton(pin), address(address), channel(channel),
          offValue(offValue), onValue(onValue) {}

  private:
    void sendOn() const override {
        Control_Surface.MIDI()->send(
            CONTROL_CHANGE, channel + channelOffset * tracksPerBank,
            address + addressOffset * tracksPerBank, onValue);
    }

    void sendOff() const override {
        Control_Surface.MIDI()->send(
            CONTROL_CHANGE, channel + channelOffset * tracksPerBank,
            address + addressOffset * tracksPerBank, onValue);
    }

    const uint8_t address;
    const uint8_t channel;
    const uint8_t offValue;
    const uint8_t onValue;
};