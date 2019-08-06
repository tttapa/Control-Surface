#pragma once

#include <Banks/BankableMIDIOutput.hpp>
#include <Def/Def.hpp>
#include <Hardware/Button.hpp>
#include <Helpers/Array.hpp>
#include <MIDI_Outputs/Abstract/MIDIOutputElement.hpp>

namespace Bankable {

/**
 * @brief   An abstract class for momentary push buttons that send MIDI events.
 *
 * The buttons are debounced.
 *
 * @see     Button
 */
template <class Sender, uint8_t NUMBER_OF_BUTTONS>
class MIDIButtons : public BankableMIDIOutput, public MIDIOutputElement {
  protected:
    /**
     * @brief   Construct a new MIDIButtons.
     *
     * @todo    Documentation
     */
    MIDIButtons(const OutputBankConfig &config,
                const Array<Button, NUMBER_OF_BUTTONS> &buttons,
                const MIDICNChannelAddress &baseAddress,
                const RelativeMIDICNChannelAddress &incrementAddress,
                const Sender &sender)
        : BankableMIDIOutput(config), buttons{buttons},
          baseAddress(baseAddress),
          incrementAddress(incrementAddress), sender{sender} {}

  public:
    void begin() final override {
        for (Button &button : buttons)
            button.begin();
    }
    void update() final override {
        MIDICNChannelAddress address = baseAddress;
        for (Button &button : buttons) {
            Button::State state = button.getState();
            if (state == Button::Falling) {
                if (!activeButtons)
                    lock(); // Don't allow changing of the bank setting
                MIDICNChannelAddress sendAddress = address + getAddressOffset();
                activeButtons++;
                sender.sendOn(sendAddress);
            } else if (state == Button::Rising) {
                MIDICNChannelAddress sendAddress = address + getAddressOffset();
                sender.sendOff(sendAddress);
                activeButtons--;
                if (!activeButtons)
                    unlock();
            }
            address += incrementAddress;
        }
    }

#ifdef INDIVIDUAL_BUTTON_INVERT
    void invert() {
        for (auto &button : buttons)
            button.invert();
    }
#endif

  private:
    Array<Button, NUMBER_OF_BUTTONS> buttons;
    const MIDICNChannelAddress baseAddress;
    const RelativeMIDICNChannelAddress incrementAddress;
    Sender sender;
    uint8_t activeButtons = 0;
};

} // namespace Bankable