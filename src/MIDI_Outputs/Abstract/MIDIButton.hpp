#pragma once

#include "../../Hardware/Button.h"
#include "DigitalMIDIOut.h"

/**
 * @brief   A class for momentary push buttons that send MIDI events.
 * 
 * The button is debounced.
 * 
 * @see     Button
 */
class MIDIButton : public DigitalMIDIOut {
  public:
    /**
     * @brief   Construct a new MIDIButton.
     * 
     * @param   pin
     *          The digital input pin with the button connected.  
     *          The internal pull-up resistor will be enabled.
     */
    MIDIButton(pin_t pin) : button{pin} {}

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
        if (state == Button::Falling) {
            sendOn();
            active = true;
        } else if (state == Button::Rising) {
            sendOff();
            active = false;
        }
    }

    bool isActive() const final override { return this->active; }

    Button button;
    bool active = false;
};