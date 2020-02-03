#pragma once

#include <AH/Containers/UniquePtr.hpp>
#include <AH/Hardware/Button.hpp>
#include <Banks/BankAddresses.hpp>
#include <Control_Surface/Control_Surface_Class.hpp>
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
     *          [0, 127], the MIDI channel [CHANNEL_1, CHANNEL_16] and Cable 
     *          Number [0, 15].
     * @param   chord
     *          The chord to play on top of the base notes.
     * @param   sender
     *          The MIDI sender to use.
     */
    template <uint8_t N>
    MIDIChordButton(const OutputBankConfig &config, pin_t pin,
                    const MIDICNChannelAddress &address, const Chord<N> &chord,
                    const Sender &sender)
        : address{config, address}, button{pin},
          newChord(AH::MakeUnique<Chord<N>>(chord)), sender{sender} {}

    void begin() override { button.begin(); }

    void update() override {
        // Lock the bank setting and get the MIDI cable number to lock
        bool unlock = address.lock();
        auto sendAddress = address.getActiveAddress();
        auto cn = sendAddress.getCableNumber();

        // Lock the MIDI cable and update the button if successful
        if (Control_Surface.try_lock_mutex(cn)) {

            AH::Button::State state = button.update();
            if (state == AH::Button::Falling) {
                if (newChord)
                    chord = std::move(newChord);
                sender.sendOn(sendAddress);
                for (int8_t offset : *chord)
                    sender.sendOn(sendAddress + offset);
                unlock = false;
            } else if (state == AH::Button::Rising) {
                sender.sendOff(sendAddress);
                for (int8_t offset : *chord)
                    sender.sendOff(sendAddress + offset);
                unlock = true;
            }

            // Unlock the MIDI cable
            Control_Surface.unlock_mutex(cn);
        }

        // Unlock the bank setting again
        if (unlock)
            address.unlock();
    }

#ifdef AH_INDIVIDUAL_BUTTON_INVERT
    void invert() { button.invert(); }
#endif

    AH::Button::State getButtonState() const { return button.getState(); }

    template <uint8_t N>
    void setChord(const Chord<N> &chord) {
        newChord = new Chord<N>(chord);
    }

  private:
    SingleAddress address;
    AH::Button button;
    AH::UniquePtr<const IChord> chord;
    AH::UniquePtr<const IChord> newChord;

  public:
    Sender sender;
};

} // namespace Bankable

END_CS_NAMESPACE