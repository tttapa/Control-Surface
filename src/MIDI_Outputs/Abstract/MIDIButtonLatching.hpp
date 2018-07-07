#pragma once

#include "../../Hardware/Button.h"
#include "DigitalMIDIOut.h"

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
    /**
     * @brief   Send the appropriate MIDI event for turning on the button.
     */
    virtual void sendOn() const = 0;

    /**
     * @brief   Send the appropriate MIDI event for turning off the button.
     */
    virtual void sendOff() const = 0;

    void refresh() final override {
        Button::State state = button.getState();
        if (state == Button::Falling || state == Button::Rising) {
            sendOn();
            sendOff();
        }
    }

    bool isActive() const final override { return false; }

    Button button;
};