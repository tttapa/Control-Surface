#pragma once

#include <Def/Def.hpp>
#include <Hardware/Button.hpp>
#include <MIDI_Outputs/Abstract/MIDIOutputElement.hpp>

BEGIN_CS_NAMESPACE

/**
 * @brief   A class for latching buttons and switches that send MIDI events.
 *
 * The button is debounced.
 *
 * @see     Button
 */
template <class Sender>
class MIDIButtonLatching : public MIDIOutputElement {
  protected:
    /**
     * @brief   Construct a new MIDIButtonLatching.
     *
     * @param   pin
     *          The digital input pin with the button connected.
     *          The internal pull-up resistor will be enabled.
     */
    MIDIButtonLatching(pin_t pin, const MIDICNChannelAddress &address,
                       const Sender &sender)
        : button{pin}, address(address), sender{sender} {}

  public:
    void begin() final override { button.begin(); }
    void update() final override {
        Button::State state = button.getState();
        if (state == Button::Falling || state == Button::Rising) {
            sender.sendOn(address);
            sender.sendOff(address);
        }
    }

  private:
    Button button;
    const MIDICNChannelAddress address;

  public:
    Sender sender;
};

END_CS_NAMESPACE