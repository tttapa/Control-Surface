#pragma once

#include <MIDI_Outputs/Abstract/MIDIChordButton.hpp>
#include <MIDI_Senders/DigitalNoteSender.hpp>

BEGIN_CS_NAMESPACE

/**
 * @brief   A class of MIDIOutputElement%s that read the input of a **momentary
 *          push button or switch**, and send out MIDI **Note** events.
 * 
 * A Note On event is sent when the button is pressed, and a Note Off
 * event is sent when the button is released.  
 * The button is debounced in software.  
 * This version cannot be banked.  
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
     *          cable number [CABLE_1, CABLE_16].
     * @param   chord
     *          The chord containing the intervals of the other notes to play.
     * @param   velocity
     *          The velocity of the MIDI Note events.
     * 
     * @tparam  N
     *          The number of notes in the chord.
     */
    template <uint8_t N>
    NoteChordButton(pin_t pin, MIDIAddress address, Chord<N> chord,
                    uint8_t velocity = 0x7F)
        : MIDIChordButton<DigitalNoteSender>(pin, address, std::move(chord),
                                             {velocity}) {}

    /// Set the velocity of the MIDI Note events.
    void setVelocity(uint8_t velocity) { this->sender.setVelocity(velocity); }
    /// Get the velocity of the MIDI Note events.
    uint8_t getVelocity() const { return this->sender.getVelocity(); }
};

END_CS_NAMESPACE