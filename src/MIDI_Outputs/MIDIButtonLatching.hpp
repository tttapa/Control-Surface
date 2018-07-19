#pragma once

#include "AbstractMIDIOutput.hpp"
#include <Hardware/Button.h>

/**
 * @brief   A class for latching buttons and switches that send MIDI events.
 * 
 * The button is debounced.
 * 
 * @see     Button
 */
template <class Sender>
class MIDIButtonLatching : public AbstractMIDIOutput {
  public:
    /**
     * @brief   Construct a new MIDIButtonLatching.
     * 
     * @param   pin
     *          The digital input pin with the button connected.  
     *          The internal pull-up resistor will be enabled.
     */
    MIDIButtonLatching(pin_t pin, uint8_t baseAddress, uint8_t baseChannel)
        : button{pin}, baseAddress(baseAddress), baseChannel(baseChannel) {}

    void update() final override {
        Button::State state = button.getState();
        if (state == Button::Falling || state == Button::Rising) {
            Sender::sendOn(baseChannel, baseAddress);
            Sender::sendOff(baseChannel, baseAddress);
        }
    }

  private:
    Button button;
    const uint8_t baseAddress;
    const uint8_t baseChannel;
};