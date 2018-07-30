#pragma once

#include <Hardware/Button.h>
#include <MIDI_Outputs/Abstract/MIDIOutputElement.hpp>
#include <Def/Def.hpp>

/**
 * @brief   An abstract class for momentary push buttons that send MIDI events.
 *
 * The button is debounced.
 *
 * @see     Button
 */
template <DigitalSendFunction sendOn, DigitalSendFunction sendOff>
class MIDIButton : public MIDIOutputElement {
  protected:
    /**
     * @brief   Construct a new MIDIButton.
     *
     * @param   pin
     *          The digital input pin with the button connected.
     *          The internal pull-up resistor will be enabled.
     */
    MIDIButton(pin_t pin, uint8_t baseAddress, uint8_t baseChannel)
        : button{pin}, baseAddress(baseAddress), baseChannel(baseChannel) {}

  public:
    void begin() final override { button.begin(); }
    void update() final override {
        Button::State state = button.getState();
        if (state == Button::Falling) {
            sendOn(baseChannel, baseAddress);
        } else if (state == Button::Rising) {
            sendOff(baseChannel, baseAddress);
        }
    }

  private:
    Button button;
    const uint8_t baseAddress;
    const uint8_t baseChannel;
};