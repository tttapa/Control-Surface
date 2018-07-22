#pragma once

#include <MIDI_Outputs/Bankable/Abstract/MIDIButton.hpp>
#include <MIDI_Senders/DigitalNoteSender.hpp>

namespace Bankable {

/**
 * @brief   A class of MIDIOutputElement%s that read the input of a momentary
 *          push button or switch, and send out MIDI Note events.
 * 
 *          A Note On event is sent when the button is pressed, and a Note Off
 *          event is sent when the button is released.  
 *          The button is debounced in software.  
 *          This version can be banked.  
 *
 * @ingroup MIDIOutputElements
 */
class NoteButton : public MIDIButton<DigitalNoteSender> {
  public:
     /**
     * @brief   Create a new Bankable NoteButton object with the given pin, note
     *          number and channel.
     * 
     * @param   pin
     *          The digital input pin to read from.  
     *          The internal pull-up resistor will be enabled.
     * @param   note
     *          The MIDI note number. [0, 127]
     * @param   channel
     *          The MIDI channel. [1, 16]
     */
    NoteButton(pin_t pin, uint8_t note, uint8_t channel = 1)
        : MIDIButton(pin, note, channel) {}
};

} // namespace Bankable