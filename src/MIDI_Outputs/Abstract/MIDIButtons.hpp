#pragma once

#include <Def/Def.hpp>
#include <Hardware/Button.h>
#include <Helpers/Array.hpp>
#include <MIDI_Outputs/Abstract/MIDIOutputElement.hpp>

/**
 * @brief   An abstract class for momentary push buttons that send MIDI events.
 *
 * The buttons are debounced.
 *
 * @see     Button
 */
template <DigitalSendFunction sendOn, DigitalSendFunction sendOff,
          uint8_t NUMBER_OF_BUTTONS>
class MIDIButtons : public MIDIOutputElement {
  protected:
    /**
     * @brief   Construct a new MIDIButtons.
     *
     * @todo    Documentation
     */
    MIDIButtons(const Array<Button, NUMBER_OF_BUTTONS> &buttons,
                const MIDICNChannelAddress &baseAddress,
                const RelativeMIDICNChannelAddress &incrementAddress = {1, 0})
        : buttons{buttons}, baseAddress(baseAddress),
          incrementAddress(incrementAddress) {}

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
                sendOn(address);
            } else if (state == Button::Rising) {
                sendOff(address);
            }
            address += incrementAddress;
        }
    }

  private:
    Array<Button, NUMBER_OF_BUTTONS> buttons;
    const MIDICNChannelAddress baseAddress;
    const RelativeMIDICNChannelAddress incrementAddress;
};