#pragma once

#include <MIDI_Outputs/Abstract/MIDIButton.hpp>
#include <MIDI_Senders/DigitalCCSender.hpp>

/**
 * @brief   A class of MIDIOutputElement%s that read the input of a **momentary
 *          push button or switch**, and send out MIDI **Control Change**
 *          events.
 * 
 *          A value of 0x7F is sent when the button is pressed, and a value of
 *          0x00 is sent when the button is released.  
 *          The button is debounced in software.  
 *          This version cannot be banked.
 *
 * @ingroup MIDIOutputElements
 */
class CCButton
    : public MIDIButton<DigitalCCSender::sendOn, DigitalCCSender::sendOff> {
  public:
    /**
     * @brief   Create a new CCButton object with the given pin,
     *          the given controller number and channel.
     *
     * @param   pin
     *          The digital input pin with the button connected.  
     *          The internal pull-up resistor will be enabled.
     * @param   controller
     *          The MIDI Controller number. [0, 119]
     * @param   channel
     *          The MIDI Channel. [1, 16]
     */
    CCButton(pin_t pin, uint8_t controller, uint8_t channel)
        : MIDIButton(pin, controller, channel) {}
};