#pragma once

#include "AbstractMIDIOutput.hpp"
#include <Hardware/Button.h>
#include <Helpers/Array.hpp>

/**
 * @brief   An abstract class for momentary push buttons that send MIDI events.
 *
 * The buttons are debounced.
 *
 * @see     Button
 */
template <uint8_t NUMBER_OF_BUTTONS, class Sender>
class MIDIButtons : public AbstractMIDIOutput {
  public:
    /**
     * @brief   Construct a new MIDIButtons.
     *
     * @todo    Documentation
     */
    MIDIButtons(const Array<Button, NUMBER_OF_BUTTONS> &buttons,
                uint8_t baseAddress, uint8_t baseChannel,
                uint8_t addressIncrement, uint8_t channelIncrement)
        : buttons{buttons}, baseAddress(baseAddress), baseChannel(baseChannel),
          increment(((channelIncrement & 0xF) << 4) |
                    (addressIncrement & 0xF)) {}

    void update() final override {
        uint8_t channel = baseChannel;
        uint8_t address = baseAddress;
        uint8_t channelIncrement = getChannelIncrement();
        uint8_t addressIncrement = getAddressIncrement();
        for (Button &button : buttons) {
            Button::State state = button.getState();
            if (state == Button::Falling) {
                Sender::sendOn(channel, address);
            } else if (state == Button::Rising) {
                Sender::sendOff(channel, address);
            }
            channel += channelIncrement;
            address += addressIncrement;
        }
    }

    uint8_t getChannelIncrement() const { return increment >> 4; }
    uint8_t getAddressIncrement() const { return increment & 0xF; }

  private:
    Array<Button, NUMBER_OF_BUTTONS> buttons;
    const uint8_t baseAddress;
    const uint8_t baseChannel;
    const uint8_t increment;
};