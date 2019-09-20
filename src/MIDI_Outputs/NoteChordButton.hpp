#pragma once

#include <MIDI_Outputs/Abstract/MIDIChordButton.hpp>
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
class NoteChordButton : public MIDIChordButton<DigitalNoteSender> {
  public:
    /**
     * @brief   Create a new Bankable NoteChordButton object with the given pin, 
     *          address and chord.
     * 
     * @param   pin
     *          The digital input pin with the button connected.
     *          The internal pull-up resistor will be enabled.
     * @param   address
     *          The MIDI address of the base note, containing the note
     *          number [0, 127], channel [CHANNEL_1, CHANNEL_16], and optional 
     *          cable number [0, 15].
     * @param   chord
     *          The chord containing the intervals of the other notes to play.
     * 
     * @tparam  N
     *          The number of notes in the chord.
     */
    template <uint8_t N>
    NoteChordButton(pin_t pin, const MIDICNChannelAddress &address,
                    const Chord<N> &chord, const DigitalNoteSender &sender)
        : MIDIChordButton<DigitalNoteSender>(pin, address, chord, sender) {}
};