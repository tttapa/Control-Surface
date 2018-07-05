#pragma once

#include "../Hardware/Button.h"
#include "Abstract/NoteOut.h"

/**
 * @brief   A class for momentary push buttons that send MIDI Note events.
 * 
 * The button is debounced.
 * 
 * @see     Button
 */
class NoteButton : public NoteOut {
  public:
    /**
     * @brief   Construct a new NoteButton.
     * 
     * @param   pin
     *          The digital input pin with the button connected.  
     *          The internal pull-up resistor will be enabled.
     * @param   address
     *          The MIDI Note number. [0, 127]
     * @param   channel
     *          The MIDI Channel. [1, 16]
     * @param   velocity
     *          The MIDI velocity value. [1, 127]
     */
    NoteButton(pin_t pin, uint8_t address, uint8_t channel,
               uint8_t velocity = 127)
        : NoteOut(address, channel, velocity), button{pin} {}

  private:
    void refresh() {
        Button::State state = button.getState();
        if (state == Button::Falling)
            sendOn();
        else if (state == Button::Rising)
            sendOff();
    }

    Button button;
};