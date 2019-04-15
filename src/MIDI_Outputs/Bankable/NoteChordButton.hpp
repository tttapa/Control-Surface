#pragma once

#include <MIDI_Outputs/Bankable/Abstract/MIDIChordButton.hpp>
#include <MIDI_Senders/DigitalNoteSender.hpp>

namespace Bankable {

/**
 * @brief   A class of MIDIOutputElement%s that read the input of a **momentary
 *          push button or switch**, and send out MIDI **Note** events.
 * 
 *          A Note On event is sent when the button is pressed, and a Note Off
 *          event is sent when the button is released.  
 *          The button is debounced in software.  
 *          This version can be banked.  
 *
 * @ingroup MIDIOutputElements
 */
class NoteChordButton : public MIDIChordButton<DigitalNoteSender::sendOn,
                                               DigitalNoteSender::sendOff> {
  public:
    /**
     * @brief   Create a new Bankable NoteChordButton object with the given pin, 
     *          note number, channel and chord.
     * 
     * @param   config
     *          The bank configuration.
     * @param   pin
     *          The digital input pin to read from.  
     *          The internal pull-up resistor will be enabled.
     * @param   address
     *          The MIDI address, including the base note.
     * @param   chord
     *          The chord to play on top of the base note.
     * 
     * @ingroup MIDIOutputElementConstructors
     */
    template <uint8_t N>
    NoteChordButton(const OutputBankConfig &config, pin_t pin,
                    const MIDICNChannelAddress &address, const Chord<N> &chord)
        : MIDIChordButton<DigitalNoteSender::sendOn,
                          DigitalNoteSender::sendOff>(config, pin, address,
                                                      chord) {}
};

} // namespace Bankable