#pragma once

#include <MIDI_Outputs/Abstract/MIDIButtonLatching.hpp>
#include <MIDI_Senders/DigitalNoteSender.hpp>

/**
 * @brief   A class of MIDIOutputElement%s that read the input of a latching
 *          push button or toggle switch, and send out MIDI Note events.
 * 
 *          When the switch changes state, two MIDI Note events are sent: first
 *          a Note On event, followed immediately by a Note Off event.  
 *          The switch is debounced in software.  
 *          This version cannot be banked.  
 *
 * @ingroup MIDIOutputElements
 */
class NoteButtonLatching
    : public MIDIButtonLatching<DigitalNoteSender::sendOn,
                                DigitalNoteSender::sendOff> {
  public:
    /**
     * @brief   Create a new NoteButtonLatching object with the given pin, note
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
    NoteButtonLatching(pin_t pin, uint8_t note, uint8_t channel = 1)
        : MIDIButtonLatching(pin, note, channel) {}
};