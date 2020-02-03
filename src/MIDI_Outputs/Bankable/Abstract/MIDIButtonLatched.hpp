#pragma once

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
template <class BankAddress, class Sender>
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

            if (newstate != state) {
                state = newstate;
                state ? sender.sendOn(sendAddress) // send MIDI
                      : sender.sendOff(sendAddress);
                unlock = !state; // if enabled, keep locked, if disabled, unlock
            }

            AH::Button::State buttonstate = button.update(); // read button
            if (buttonstate == AH::Button::Falling) {        // if pressed
                newstate = !state;                           // toggle the state
                state = newstate;
                state ? sender.sendOn(sendAddress) // send MIDI
                      : sender.sendOff(sendAddress);
                unlock = !state; // if enabled, keep locked, if disabled, unlock
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
    bool toggleState() {
        setState(!getNewState());
        return getNewState();
    }

    /// Get the current state.
    bool getState() const { return state; }

    /// Get the new state (that will be sent over MIDI when the MIDI output
    /// becomes available).
    bool getNewState() const { return newstate; }

    /// Set the state to the given value.
    /// @note   The MIDI message is not actually sent until the next time when
    ///         `update` is called and the MIDI output is available.
    void setState(bool state) { this->newstate = state; }

#ifdef AH_INDIVIDUAL_BUTTON_INVERT
    void invert() { button.invert(); }
#endif

    /// Get the state of the underlying button.
    AH::Button::State getButtonState() const { return button.getState(); }

  private:
    void sendState() {
        if (state) {
            sender.sendOn(address.getActiveAddress());
        } else {
            sender.sendOff(address.getActiveAddress());
            address.unlock();
        }
    }

  private:
    BankAddress address;
    AH::Button button;
    bool state = false;
    bool newstate = false;

  public:
    Sender sender;
};

} // namespace Bankable

END_CS_NAMESPACE