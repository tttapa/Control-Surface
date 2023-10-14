/* ✔ */

#pragma once

#include <AH/Hardware/Button.hpp>
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
    MIDIButtonLatched(pin_t pin, MIDIAddress address, const Sender &sender)
        : button(pin), address(address), sender(sender) {}

  public:
    void begin() final override { button.begin(); }
    void update() final override {
        AH::Button::State state = button.update();
        if (state == AH::Button::Falling)
            toggleState();
    }

    /// Flip the state (on → off or off → on).
    /// Sends the appropriate MIDI event.
    bool toggleState() {
        setState(!getState());
        return getState();
    }

    /// Get the current state.
    bool getState() const { return state; }

    /// Set the state to the given value.
    /// Sends the appropriate MIDI event.
    void setState(bool state) {
        this->state = state;
        state ? sender.sendOn(address) : sender.sendOff(address);
    }

    /// @see @ref AH::Button::invert()
    void invert() { button.invert(); }

    /// Get the state of the underlying button.
    AH::Button::State getButtonState() const { return button.getState(); }

    /// Get the MIDI address.
    MIDIAddress getAddress() const { return this->address; }
    /// Set the MIDI address. Has unexpected consequences if used while the
    /// button is active. Use banks if you need to support that.
    void setAddressUnsafe(MIDIAddress address) { this->address = address; }

  private:
    AH::Button button;
    const MIDIAddress address;
    bool state = false;

  public:
    Sender sender;
};

END_CS_NAMESPACE