#pragma once

#include <MIDI_Outputs/Abstract/DigitalCCOut.hpp>
#include <MIDI_Outputs/Abstract/MIDIButton.hpp>

/**
 * @brief   A class for momentary push buttons that send MIDI Controller change
 *          events.
 * 
 * The button is debounced.
 * 
 * @see     Button
 */
class CCButton : public MIDIButton, public DigitalCCOut {
  public:
    /**
     * @brief   Construct a new CCButton.
     * 
     * @param   pin
     *          The digital input pin with the button connected.  
     *          The internal pull-up resistor will be enabled.
     * @param   controller
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
    CCButton(pin_t pin, uint8_t controller, uint8_t channel,
             uint8_t offValue = 0, uint8_t onValue = 127)
        : MIDI_Output_Addressable(controller, channel), MIDIButton(pin),
          DigitalCCOut(offValue, onValue) {}
};