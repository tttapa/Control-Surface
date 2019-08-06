#pragma once

#include <MIDI_Outputs/Abstract/MIDIButton.hpp>
#include <MIDI_Senders/DigitalNoteSender.hpp>

/**
 * @brief   A class of MIDIOutputElement%s that read the input of a **momentary
 *          push button or switch**, and send out MIDI **Note** events.
 * 
 *          A Note On event is sent when the button is pressed, and a Note Off
 *          event is sent when the button is released.  
 *          The button is debounced in software.  
 *          This version cannot be banked.  
 *
 * @ingroup MIDIOutputElements
 */
class NoteButton : public MIDIButton<DigitalNoteSender> {
  public:
    /**
     * @brief   Create a new NoteButton object with the given pin, note number
     *          and channel.
     * 
     * @param   pin
     *          The digital input pin to read from.  
     *          The internal pull-up resistor will be enabled.
     * @param   note
     *          The MIDI note number. [0, 127]
     * @param   channel
     *          The MIDI channel. [1, 16]
     * 
     * @ingroup MIDIOutputElementConstructors
     */
    NoteButton(pin_t pin, const MIDICNChannelAddress &address,
               const DigitalNoteSender &sender = {})
        : MIDIButton(pin, address, sender) {}
};