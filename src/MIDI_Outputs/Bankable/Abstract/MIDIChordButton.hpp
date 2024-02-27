#pragma once

#include <AH/Hardware/Button.hpp>
#include <AH/STL/memory> // std::unique_ptr
#include <Banks/BankAddresses.hpp>
#include <Def/Def.hpp>
#include <MIDI_Constants/Chords/Chords.hpp>
#include <MIDI_Outputs/Abstract/MIDIOutputElement.hpp>

BEGIN_CS_NAMESPACE

namespace Bankable {

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
     * @brief   Construct a new bankable MIDIChordButton.
     *
     * @param   config
     *          The bank configuration to use: the bank to add this element to,
     *          and whether to change the address, channel or cable number.
     * @param   pin
     *          The digital input pin with the button connected.
     *          The internal pull-up resistor will be enabled.
     * @param   address
     *          The address of the base note, containing the note number 
     *          [0, 127], the MIDI channel [Channel_1, Channel_16] and Cable 
     *          Number [Cable_1, Cable_16].
     * @param   chord
     *          The chord to play on top of the base notes.
     * @param   sender
     *          The MIDI sender to use.
     */
    template <uint8_t N>
    MIDIChordButton(OutputBankConfig<> config, pin_t pin, MIDIAddress address,
                    Chord<N> chord, const Sender &sender)
        : address {config, address}, button(pin),
          newChord(AH::make_unique<Chord<N>>(std::move(chord))),
          sender(sender) {}

    void begin() override { button.begin(); }
    void update() override {
        AH::Button::State state = button.update();
        if (state == AH::Button::Falling) {
            if (newChord)
                chord = std::move(newChord);
            address.lock();
            auto sendAddress = address.getActiveAddress();
            sender.sendOn(sendAddress);
            for (int8_t offset : *chord)
                sender.sendOn(sendAddress + offset);
        } else if (state == AH::Button::Rising) {
            auto sendAddress = address.getActiveAddress();
            sender.sendOff(sendAddress);
            for (int8_t offset : *chord)
                sender.sendOff(sendAddress + offset);
            address.unlock();
        }
    }

    /// @see @ref AH::Button::invert()
    void invert() { button.invert(); }

    AH::Button::State getButtonState() const { return button.getState(); }

    template <uint8_t N>
    void setChord(Chord<N> chord) {
        newChord = AH::make_unique<Chord<N>>(std::move(chord));
    }

  private:
    SingleAddress address;
    AH::Button button;
    std::unique_ptr<const IChord> chord;
    std::unique_ptr<const IChord> newChord;

  public:
    Sender sender;
};

} // namespace Bankable

END_CS_NAMESPACE
