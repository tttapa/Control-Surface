#pragma once

#include <AH/Hardware/Button.hpp>
#include <AH/STL/memory> // std::unique_ptr
#include <Def/Def.hpp>
#include <MIDI_Constants/Chords/Chords.hpp>
#include <MIDI_Outputs/Abstract/MIDIOutputElement.hpp>

BEGIN_CS_NAMESPACE

/**
 * @brief   An abstract class for momentary push buttons that send multiple MIDI
 *          events.
 *
 * The button is debounced.
 *
 * @see     AH::Button
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
     *          number [0, 127], channel [Channel_1, Channel_16], and optional 
     *          cable number [Cable_1, Cable_16].
     * @param   chord
     *          The chord containing the intervals of the other notes to play.
     * @param   sender
     *          The MIDI Note sender to use.
     * 
     * @tparam  N
     *          The number of notes in the chord.
     */
    template <uint8_t N>
    MIDIChordButton(pin_t pin, MIDIAddress address, Chord<N> chord,
                    const Sender &sender)
        : button(pin), address(address),
          newChord(AH::make_unique<Chord<N>>(std::move(chord))),
          sender(sender) {}
    // TODO: can I somehow get rid of the dynamic memory allocation here?

    void begin() final override { button.begin(); }
    void update() final override {
        AH::Button::State state = button.update();
        MIDIAddress sendAddress = address;
        if (state == AH::Button::Falling) {
            if (newChord)
                chord = std::move(newChord);
            sender.sendOn(sendAddress);
            for (int8_t offset : *chord)
                sender.sendOn(sendAddress + offset);
        } else if (state == AH::Button::Rising) {
            sender.sendOff(sendAddress);
            for (int8_t offset : *chord)
                sender.sendOff(sendAddress + offset);
        }
    }

    /// @see @ref AH::Button::invert()
    void invert() { button.invert(); }

    AH::Button::State getButtonState() const { return button.getState(); }

    /// Change the chord. Can be used safely while the push button is pressed.
    template <uint8_t N>
    void setChord(Chord<N> chord) {
        newChord = AH::make_unique<Chord<N>>(std::move(chord));
    }

    /// Get the MIDI address.
    MIDIAddress getAddress() const { return this->address; }
    /// Set the MIDI address. Has unexpected consequences if used while the
    /// push button is pressed. Use banks if you need to support that.
    void setAddressUnsafe(MIDIAddress address) { this->address = address; }

  private:
    AH::Button button;
    MIDIAddress address;
    std::unique_ptr<const IChord> chord;
    std::unique_ptr<const IChord> newChord;

  public:
    Sender sender;
};

END_CS_NAMESPACE
