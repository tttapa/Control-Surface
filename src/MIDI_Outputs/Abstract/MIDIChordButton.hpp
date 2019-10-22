#pragma once

#include <Def/Def.hpp>
#include <Hardware/Button.hpp>
#include <Helpers/unique_ptr.hpp>
#include <MIDI_Constants/Chords/Chords.hpp>
#include <MIDI_Outputs/Abstract/MIDIOutputElement.hpp>

BEGIN_CS_NAMESPACE

/**
 * @brief   An abstract class for momentary push buttons that send multiple MIDI
 *          events.
 *
 * The button is debounced.
 *
 * @see     Button
 */
template <class Sender>
class MIDIChordButton : public MIDIOutputElement {
  public:
    /**
     * @brief   Construct a new MIDIChordButton.
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
     * @param   sender
     *          The MIDI Note sender to use.
     * 
     * @tparam  N
     *          The number of notes in the chord.
     */
    template <uint8_t N>
    MIDIChordButton(pin_t pin, const MIDICNChannelAddress &address,
                    const Chord<N> &chord, const Sender &sender)
        : button{pin}, address(address),
          newChord(make_unique<Chord<N>>(chord)), sender{sender} {}
    // TODO: can I somehow get rid of the dynamic memory allocation here?

    void begin() final override { button.begin(); }
    void update() final override {
        Button::State state = button.getState();
        MIDICNChannelAddress sendAddress = address;
        if (state == Button::Falling) {
            if (newChord)
                chord = move(newChord);
            sender.sendOn(sendAddress);
            for (int8_t offset : *chord)
                sender.sendOn(sendAddress + offset);
        } else if (state == Button::Rising) {
            sender.sendOff(sendAddress);
            for (int8_t offset : *chord)
                sender.sendOff(sendAddress + offset);
        }
    }

#ifdef INDIVIDUAL_BUTTON_INVERT
    void invert() { button.invert(); }
#endif

    template <uint8_t N>
    void setChord(const Chord<N> &chord) {
        newChord = new Chord<N>(chord);
    }

  private:
    Button button;
    const MIDICNChannelAddress address;
    unique_ptr<const IChord> chord;
    unique_ptr<const IChord> newChord;

  public:
    Sender sender;
};

END_CS_NAMESPACE