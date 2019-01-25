/* ✔ */

#pragma once

#include <Def/Def.hpp>
#include <Hardware/Button.hpp>
#include <MIDI_Outputs/Abstract/MIDIOutputElement.hpp>

/**
 * @brief   A class for momentary buttons and switches that send MIDI events.
 *
 * The button is debounced, and the button is latched (press once to enable, 
 * press again to disable).
 *
 * @see     Button
 */
template <DigitalSendFunction sendOn, DigitalSendFunction sendOff>
class MIDIButtonLatched : public MIDIOutputElement {
  protected:
    /**
     * @brief   Create a new MIDIButtonLatched object on the given pin and with 
     *          address.
     * 
     * @param   pin
     *          The digital input pin to read from.  
     *          The internal pull-up resistor will be enabled.
     * @param   address
     *          The MIDI address containing the note number [0, 127], channel
     *          [1, 16], and optional cable index number.
     */
    MIDIButtonLatched(pin_t pin, const MIDICNChannelAddress &address)
        : button{pin}, address{address} {}

  public:
    void begin() final override { button.begin(); }
    void update() final override {
        Button::State state = button.getState();
        if (state == Button::Falling)
            toggleState();
    }

    bool toggleState() {
        setState(!getState());
        return getState();
    }
    bool getState() const { return state; }
    void setState(bool state) {
        this->state = state;
        state ? sendOn(address) : sendOff(address);
    }

  private:
    Button button;
    const MIDICNChannelAddress address;
    bool state = false;
};