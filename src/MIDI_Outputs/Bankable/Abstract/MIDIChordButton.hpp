#pragma once

#include <Banks/BankableMIDIOutput.hpp>
#include <Def/Def.hpp>
#include <Hardware/Button.hpp>
#include <Helpers/unique_ptr.hpp>
#include <MIDI_Constants/Chords/Chords.hpp>
#include <MIDI_Outputs/Abstract/MIDIOutputElement.hpp>

namespace Bankable {

/**
 * @brief   An abstract class for momentary push buttons that send multiple MIDI
 *          events.
 *
 * The button is debounced.
 *
 * @see     Button
 */
template <DigitalSendFunction sendOn, DigitalSendFunction sendOff>
class MIDIChordButton : public BankableMIDIOutput, public MIDIOutputElement {
  public:
    /**
     * @brief   Construct a new bankable MIDIChordButton.
     *
     * @param   config
     * @param   pin
     *          The digital input pin with the button connected.
     *          The internal pull-up resistor will be enabled.
     * @param   address
     * @param   chord
     */
    template <uint8_t N>
    MIDIChordButton(const OutputBankConfig &config, pin_t pin,
                    const MIDICNChannelAddress &address, const Chord<N> &chord)
        : BankableMIDIOutput{config}, button{pin}, address(address),
          newChord(new Chord<N>(chord)) {}
    // TODO: can I somehow get rid of the dynamic memory allocation here?

    void begin() final override { button.begin(); }
    void update() final override {
        Button::State state = button.getState();
        MIDICNChannelAddress sendAddress = address;
        if (state == Button::Falling) {
            if (newChord)
                chord = move(newChord);
            lock();
            sendAddress += getAddressOffset();
            sendOn(sendAddress);
            for (int8_t offset : *chord)
                sendOn(sendAddress + offset);
        } else if (state == Button::Rising) {
            sendAddress += getAddressOffset();
            sendOff(sendAddress);
            for (int8_t offset : *chord)
                sendOff(sendAddress + offset);
            unlock();
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
};

} // namespace Bankable