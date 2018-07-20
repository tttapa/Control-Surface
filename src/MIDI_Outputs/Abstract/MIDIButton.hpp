#pragma once

#include <MIDI_Outputs/Abstract/AbstractMIDIOutput.hpp>
#include <Hardware/Button.h>

/**
 * @brief   An abstract class for momentary push buttons that send MIDI events.
 *
 * The button is debounced.
 *
 * @see     Button
 */
template <class Sender>
class MIDIButton : public AbstractMIDIOutput {
  public:
    /**
     * @brief   Construct a new MIDIButton.
     *
     * @param   pin
     *          The digital input pin with the button connected.
     *          The internal pull-up resistor will be enabled.
     */
    MIDIButton(pin_t pin, uint8_t baseAddress, uint8_t baseChannel)
        : button{pin}, baseAddress(baseAddress), baseChannel(baseChannel) {}

    void update() final override {
        Button::State state = button.getState();
        if (state == Button::Falling) {
            Sender::sendOn(baseChannel, baseAddress);
        } else if (state == Button::Rising) {
            Sender::sendOff(baseChannel, baseAddress);
        }
    }

  private:
    Button button;
    const uint8_t baseAddress;
    const uint8_t baseChannel;
};