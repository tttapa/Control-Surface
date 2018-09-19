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
                const MIDICNChannelAddress &address, uint8_t addressIncrement,
                uint8_t channelIncrement)
        : BankableMIDIOutput(config), buttons{buttons}, address(address),
          increment((channelIncrement << 4) | (addressIncrement & 0xF)) {}

  public:
    void begin() final override {
        for (Button &button : buttons)
            button.begin();
    }
    void update() final override {
        MIDICNChannelAddress sendAddress = this->address;
        sendAddress += getAddressOffset();
        uint8_t channel = sendAddress.getChannel();
        uint8_t address = sendAddress.getAddress();
        uint8_t channelIncrement = getChannelIncrement();
        uint8_t addressIncrement = getAddressIncrement();
        for (Button &button : buttons) {
            Button::State state = button.getState();
            if (state == Button::Falling) {
                if (!activeButtons)
                    lock(); // Don't allow changing of the bank setting
                activeButtons++;
                sendOn(channel, address);
            } else if (state == Button::Rising) {
                sendOff(channel, address);
                activeButtons--;
                if (!activeButtons)
                    unlock();
            }
            channel += channelIncrement;
            address += addressIncrement;
        }
    }

    uint8_t getChannelIncrement() const { return increment >> 4; }
    uint8_t getAddressIncrement() const { return increment & 0xF; }

  private:
    Array<Button, NUMBER_OF_BUTTONS> buttons;
    const MIDICNChannelAddress address;
    const uint8_t increment;
    uint8_t activeButtons = 0;
};

} // namespace Bankable