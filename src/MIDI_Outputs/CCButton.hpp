#pragma once

#include <MIDI_Outputs/Abstract/MIDIButton.hpp>
#include <MIDI_Senders/DigitalCCSender.hpp>

BEGIN_CS_NAMESPACE

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
class CCButton : public MIDIButton<DigitalCCSender> {
  public:
    /**
     * @brief   Create a new CCButton object with the given pin,
     *          the given controller number and channel.
     *
     * @param   pin
     *          The digital input pin with the button connected.  
     *          The internal pull-up resistor will be enabled.
     * @param   address
     *          The MIDI address containing the controller number [0, 119], 
     *          channel [CHANNEL_1, CHANNEL_16], and optional cable number 
     *          [CABLE_1, CABLE_16].
     * @param   sender
     *          The MIDI sender to use.
     */
    CCButton(pin_t pin, MIDIAddress address, const DigitalCCSender &sender = {})
        : MIDIButton(pin, address, sender) {}
};

END_CS_NAMESPACE