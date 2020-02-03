/* ✔ */

#pragma once

#include <AH/Hardware/Button.hpp>
#include <Control_Surface/Control_Surface_Class.hpp>
#include <Def/Def.hpp>
#include <MIDI_Outputs/Abstract/MIDIOutputElement.hpp>

BEGIN_CS_NAMESPACE

/**
 * @brief   A class for momentary buttons and switches that send MIDI events.
 *
 * The button is debounced, and the button is latched (press once to enable, 
 * press again to disable) (toggle).
 *
 * @see     Button
 */
template <class Sender>
class MIDIButtonLatched : public MIDIOutputElement {
  protected:
    /**
     * @brief   Create a new MIDIButtonLatched object on the given pin and 
     *          address.
     * 
     * @param   pin
     *          The digital input pin to read from.  
     *          The internal pull-up resistor will be enabled.
     * @param   address
     *          The MIDI address to send to.
     * @param   sender
     *          The MIDI sender to use.
     */
    MIDIButtonLatched(pin_t pin, const MIDICNChannelAddress &address,
                      const Sender &sender)
        : button{pin}, address{address}, sender{sender} {}

  public:
    void begin() final override { button.begin(); }

    void update() final override {
        auto cn = address.getCableNumber();
        if (!Control_Surface.try_lock_mutex(cn))
            return;

        if (newstate != state) {
            state = newstate;
            sendState();
        }

        AH::Button::State buttonstate = button.update();
        if (buttonstate == AH::Button::Falling) {
            state = (newstate = !state);
            sendState();
        }

        Control_Surface.unlock_mutex(cn);
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
        state ? sender.sendOn(address) : sender.sendOff(address);
    }

  private:
    AH::Button button;
    const MIDICNChannelAddress address;
    bool state = false;
    bool newstate = false;

  public:
    Sender sender;
};

END_CS_NAMESPACE