#pragma once

#include "../Hardware/Button.h"
#include "Abstract/CCOut.h"

/**
 * @brief   A class for momentary push buttons that send MIDI Controller change
 *          events.
 * 
 * The button is debounced.
 * 
 * @see     Button
 */
class CCButton : public DigitalCCOut {
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
        : DigitalCCOut(address, channel, offValue, onValue), button{pin} {}

  private:
    void refresh() override {
        Button::State state = button.getState();
        if (state == Button::Falling)
            sendOn();
        else if (state == Button::Rising)
            sendOff();
    }

    Button button;
};