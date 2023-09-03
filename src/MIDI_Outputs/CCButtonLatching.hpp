#pragma once

#include <MIDI_Outputs/Abstract/MIDIButtonLatching.hpp>
#include <MIDI_Senders/DigitalCCSender.hpp>

BEGIN_CS_NAMESPACE

/**
 * @brief   A class of MIDIOutputElement%s that read the input of a **latching
 *          push button or toggle switch**, and send out MIDI **Control Change**
 *          events.
 * 
 * When the switch changes state, two MIDI events are sent: the first
 * one with a value of 0x7F, followed immediately by a second one with
 * a value of 0x00.  
 * The switch is debounced in software.  
 * This version cannot be banked.
 *
 * @ingroup MIDIOutputElements
 */
class CCButtonLatching : public MIDIButtonLatching<DigitalCCSender> {
  public:
    /**
     * @brief   Create a new CCButtonLatching object with the given pin,
     *          the given controller number and channel.
     *
     * @param   pin
     *          The digital input pin with the button connected.  
     *          The internal pull-up resistor will be enabled.
     * @param   address
     *          The MIDI address containing the controller number [0, 119], 
     *          channel [Channel_1, Channel_16], and optional cable number 
     *          [Cable_1, Cable_16].
     * @param   sender
     *          The MIDI sender to use.
     */
    CCButtonLatching(pin_t pin, MIDIAddress address,
                     const DigitalCCSender &sender = {})
        : MIDIButtonLatching(pin, address, sender) {}
};

END_CS_NAMESPACE