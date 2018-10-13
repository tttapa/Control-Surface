#pragma once

#include <Banks/BankableMIDIOutput.hpp>
#include <Def/Def.hpp>
#include <Hardware/Button.h>
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
template <DigitalSendFunction sendOn, DigitalSendFunction sendOff,
          uint8_t NUMBER_OF_BUTTONS>
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
                const RelativeMIDICNChannelAddress &incrementAddress)
        : BankableMIDIOutput(config), buttons{buttons},
          baseAddress(baseAddress), incrementAddress(incrementAddress) {}

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
                sendOn(sendAddress);
            } else if (state == Button::Rising) {
                MIDICNChannelAddress sendAddress = address + getAddressOffset();
                sendOff(sendAddress);
                activeButtons--;
                if (!activeButtons)
                    unlock();
            }
            address += incrementAddress;
        }
    }

  private:
    Array<Button, NUMBER_OF_BUTTONS> buttons;
    const MIDICNChannelAddress baseAddress;
    const RelativeMIDICNChannelAddress incrementAddress;
    uint8_t activeButtons = 0;
};

} // namespace Bankable