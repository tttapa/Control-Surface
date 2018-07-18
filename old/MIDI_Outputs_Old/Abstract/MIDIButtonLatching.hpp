#pragma once

#include "DigitalMIDIOut.hpp"
#include <Hardware/Button.h>

/**
 * @brief   A class for latching buttons and switches that send MIDI events.
 * 
 * The button is debounced.
 * 
 * @see     Button
 */
class MIDIButtonLatching : public DigitalMIDIOut {
  public:
    /**
     * @brief   Construct a new MIDIButtonLatching.
     * 
     * @param   pin
     *          The digital input pin with the button connected.  
     *          The internal pull-up resistor will be enabled.
     */
    MIDIButtonLatching(pin_t pin) : button{pin} {}

  private:
    void refresh() final override {
        Button::State state = button.getState();
        if (state == Button::Falling || state == Button::Rising) {
            sendOn();
            sendOff();
        }
    }

    Button button;
};