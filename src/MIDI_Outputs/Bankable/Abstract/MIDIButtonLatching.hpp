#pragma once

#include <Banks/BankableMIDIOutputAddressable.hpp>
#include <Hardware/Button.h>
#include <MIDI_Outputs/Abstract/MIDIOutputElement.hpp>

namespace Bankable {

/**
 * @brief   A class for latching buttons and switches that send MIDI events.
 *
 * The button is debounced.
 *
 * @see     Button
 */
template <class Sender>
class MIDIButtonLatching : public MIDIOutputElement,
                           public BankableMIDIOutputAddressable {
  protected:
    /**
     * @brief   Construct a new MIDIButtonLatching.
     *
     * @param   pin
     *          The digital input pin with the button connected.
     *          The internal pull-up resistor will be enabled.
     */
    MIDIButtonLatching(const BankConfigAddressable &config, pin_t pin,
                       uint8_t baseAddress, uint8_t baseChannel)
        : BankableMIDIOutputAddressable(config), button{pin},
          baseAddress(baseAddress), baseChannel(baseChannel) {}

  public:
    void begin() final override { button.begin(); }
    void update() final override {
        Button::State state = button.getState();
        if (state == Button::Falling || state == Button::Rising) {
            Sender::sendOn(getChannel(baseChannel), getAddress(baseAddress));
            Sender::sendOff(getChannel(baseChannel), getAddress(baseAddress));
        }
    }

  private:
    Button button;
    const uint8_t baseAddress;
    const uint8_t baseChannel;
};

} // namespace Bankable