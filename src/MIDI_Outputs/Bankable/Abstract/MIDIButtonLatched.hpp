#pragma once

#include <AH/Containers/BitArray.hpp>
#include <AH/Hardware/Button.hpp>
#include <Banks/BankableMIDIOutput.hpp>
#include <Control_Surface/Control_Surface_Class.hpp>
#include <Def/Def.hpp>
#include <MIDI_Outputs/Abstract/MIDIOutputElement.hpp>

BEGIN_CS_NAMESPACE

namespace Bankable {

/**
 * @brief   A class for momentary buttons and switches that send MIDI events.
 *
 * The button is debounced, and the button is latched (press once to enable, 
 * press again to disable) (toggle).
 *
 * @see     AH::Button
 */
template <uint8_t NumBanks, class BankAddress, class Sender>
class MIDIButtonLatched : public MIDIOutputElement {
  protected:
    /**
     * @brief   Create a new bankable MIDIButtonLatched object on the given pin
     *          and address.
     * 
     * @param   bankAddress
     *          The bankable MIDI address to send to.
     * @param   pin
     *          The digital input pin with the button connected.
     *          The internal pull-up resistor will be enabled.
     * @param   sender
     *          The MIDI sender to use.
     */
    MIDIButtonLatched(const BankAddress &bankAddress, pin_t pin,
                      const Sender &sender)
        : address{bankAddress}, button{pin}, sender{sender} {}

  public:
    void begin() override { button.begin(); }

    void update() override {
        // Lock the bank setting and get the MIDI cable number to lock
        bool unlock = address.lock();
        auto sendAddress = address.getActiveAddress();
        auto cn = sendAddress.getCableNumber();

        // Lock the MIDI cable and update the button if successful
        if (Control_Surface.try_lock_mutex(cn)) {

            for (setting_t i = 0; i < NumBanks; ++i) {
                if (newstates.get(i) != states.get(i)) {
                    states.set(i, newstates.get(i));
                    states.get(i) ? sender.sendOn(sendAddress) // send MIDI
                                  : sender.sendOff(sendAddress);
                }
            }

            AH::Button::State buttonstate = button.update(); // read button
            if (buttonstate == AH::Button::Falling) {        // if pressed
                setting_t activeBank = address.getSelection();
                bool newstate = toggleState(activeBank);
                states.set(activeBank, newstate);
                newstate ? sender.sendOn(sendAddress) // send MIDI
                         : sender.sendOff(sendAddress);
            }

            // Unlock the MIDI cable
            Control_Surface.unlock_mutex(cn);
        }

        // Unlock the bank setting again
        if (unlock)
            address.unlock();
    }

    /// Flip the state (on → off or off → on).
    /// @note   The MIDI message is not actually sent until the next time when
    ///         `update` is called and the MIDI output is available.
    /// @return The new state.
    bool toggleState(setting_t bank) {
        bool newstate = !getNewState(bank);
        setState(bank, newstate);
        return newstate;
    }

    /// Get the current state.
    bool getState(setting_t bank) const { return states.get(bank); }

    /// Get the new state (that will be sent over MIDI when the MIDI output
    /// becomes available).
    bool getNewState(setting_t bank) const { return newstates.get(bank); }

    /// Set the state to the given value.
    /// @note   The MIDI message is not actually sent until the next time when
    ///         `update` is called and the MIDI output is available.
    void setState(setting_t bank, bool state) { newstates.set(bank, state); }

#ifdef AH_INDIVIDUAL_BUTTON_INVERT
    void invert() { button.invert(); }
#endif

    /// Get the state of the underlying button.
    AH::Button::State getButtonState() const { return button.getState(); }

  private:
    BankAddress address;
    AH::Button button;
    AH::BitArray<NumBanks> states;
    AH::BitArray<NumBanks> newstates;

  public:
    Sender sender;
};

} // namespace Bankable

END_CS_NAMESPACE