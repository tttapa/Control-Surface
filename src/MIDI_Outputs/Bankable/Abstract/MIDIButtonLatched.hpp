#pragma once

#include <AH/Containers/BitArray.hpp>
#include <AH/Hardware/Button.hpp>
#include <Banks/BankableAddresses.hpp>
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
    MIDIButtonLatched(BankAddress bankAddress, pin_t pin, const Sender &sender)
        : address(bankAddress), button(pin), sender(sender) {}

  public:
    void begin() override { button.begin(); }

    void update() override {
        AH::Button::State state = button.update();
        if (state == AH::Button::Falling)
            toggleState();
    }

    bool toggleState() {
        bool newstate = !getState();
        setState(newstate);
        return newstate;
    }

    bool getState() const { return states.get(address.getSelection()); }

    void setState(bool state) {
        states.set(address.getSelection(), state);
        if (state) {
            sender.sendOn(address.getActiveAddress());
        } else {
            sender.sendOff(address.getActiveAddress());
        }
    }

    AH::Button::State getButtonState() const { return button.getState(); }

  protected:
    BankAddress address;
    AH::Button button;
    AH::BitArray<NumBanks> states;

  public:
    Sender sender;
};

} // namespace Bankable

END_CS_NAMESPACE