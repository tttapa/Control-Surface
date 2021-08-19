#pragma once

#include <AH/Containers/Array.hpp>
#include <AH/Hardware/Button.hpp>
#include <Banks/BankableAddresses.hpp>
#include <Def/Def.hpp>
#include <MIDI_Outputs/Abstract/MIDIOutputElement.hpp>

BEGIN_CS_NAMESPACE

namespace Bankable {

/**
 * @brief   An abstract class for momentary push buttons that send MIDI events.
 *
 * The buttons are debounced.
 * 
 * @todo    Use BankAddresses?
 *
 * @see     Button
 */
template <class BankAddress, class Sender, uint8_t NumButtons>
class MIDIButtons : public MIDIOutputElement {
  protected:
    /**
     * @brief   Construct a new MIDIButtons.
     *
     * @todo    Documentation
     */
    MIDIButtons(BankAddress bankAddress,
                const Array<AH::Button, NumButtons> &buttons,
                RelativeMIDIAddress incrementAddress, const Sender &sender)
        : address(bankAddress), buttons(buttons),
          incrementAddress(incrementAddress), sender(sender) {}

  public:
    void begin() final override {
        for (auto &button : buttons)
            button.begin();
    }
    void update() final override {
        RelativeMIDIAddress offset = {0, 0, 0};
        for (auto &button : buttons) {
            AH::Button::State state = button.update();
            if (state == AH::Button::Falling) {
                if (!activeButtons)
                    address.lock(); // Don't allow changing of the bank setting
                MIDIAddress sendAddress = address.getActiveAddress() + offset;
                sender.sendOn(sendAddress);
                activeButtons++;
            } else if (state == AH::Button::Rising) {
                MIDIAddress sendAddress = address.getActiveAddress() + offset;
                sender.sendOff(sendAddress);
                activeButtons--;
                if (!activeButtons)
                    address.unlock();
            }
            offset += incrementAddress;
        }
    }

    /// @see @ref AH::Button::invert()
    void invert() {
        for (auto &button : buttons)
            button.invert();
    }

    AH::Button::State getButtonState(size_t index) const {
        return buttons[index].getState();
    }

  protected:
    BankAddress address;
    Array<AH::Button, NumButtons> buttons;
    RelativeMIDIAddress incrementAddress;
    uint8_t activeButtons = 0;

  public:
    Sender sender;
};

} // namespace Bankable

END_CS_NAMESPACE