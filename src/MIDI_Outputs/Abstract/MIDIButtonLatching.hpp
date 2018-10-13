#pragma once

#include <Hardware/Button.h>
#include <MIDI_Outputs/Abstract/MIDIOutputElement.hpp>
#include <Def/Def.hpp>

/**
 * @brief   A class for latching buttons and switches that send MIDI events.
 *
 * The button is debounced.
 *
 * @see     Button
 */
template <DigitalSendFunction sendOn, DigitalSendFunction sendOff>
class MIDIButtonLatching : public MIDIOutputElement {
  protected:
    /**
     * @brief   Construct a new MIDIButtonLatching.
     *
     * @param   pin
     *          The digital input pin with the button connected.
     *          The internal pull-up resistor will be enabled.
     */
    MIDIButtonLatching(pin_t pin, const MIDICNChannelAddress &address)
        : button{pin}, address(address) {}

  public:
    void begin() final override { button.begin(); }
    void update() final override {
        Button::State state = button.getState();
        if (state == Button::Falling || state == Button::Rising) {
            sendOn(address);
            sendOff(address);
        }
    }

  private:
    Button button;
    const MIDICNChannelAddress address;
};