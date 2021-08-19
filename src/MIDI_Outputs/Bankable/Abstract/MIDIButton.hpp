#pragma once

#include <AH/Hardware/Button.hpp>
#include <Banks/BankableAddresses.hpp>
#include <Def/Def.hpp>
#include <MIDI_Outputs/Abstract/MIDIOutputElement.hpp>

BEGIN_CS_NAMESPACE

namespace Bankable {

/**
 * @brief   An abstract class for momentary push buttons that send MIDI events.
 *
 * The button is debounced.
 *
 * @see     Button
 */
template <class BankAddress, class Sender>
class MIDIButton : public MIDIOutputElement {
  public:
    /**
     * @brief   Construct a new bankable MIDIButton.
     *
     * @param   bankAddress
     *          The bankable MIDI address to send to.
     * @param   pin
     *          The digital input pin with the button connected.
     *          The internal pull-up resistor will be enabled.
     * @param   sender
     *          The MIDI sender to use.
     */
    MIDIButton(BankAddress bankAddress, pin_t pin, const Sender &sender)
        : address(bankAddress), button(pin), sender(sender) {}

    void begin() override { button.begin(); }
    void update() override {
        AH::Button::State state = button.update();
        if (state == AH::Button::Falling) {
            address.lock();
            sender.sendOn(address.getActiveAddress());
        } else if (state == AH::Button::Rising) {
            sender.sendOff(address.getActiveAddress());
            address.unlock();
        }
    }

    /// @see @ref AH::Button::invert()
    void invert() { button.invert(); }

    AH::Button::State getButtonState() const { return button.getState(); }

  protected:
    BankAddress address;
    AH::Button button;

  public:
    Sender sender;
};

} // namespace Bankable

END_CS_NAMESPACE