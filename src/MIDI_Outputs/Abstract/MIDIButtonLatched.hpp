/* ✔ */

#pragma once

#include <Def/Def.hpp>
#include <Hardware/Button.hpp>
#include <MIDI_Outputs/Abstract/MIDIOutputElement.hpp>

/**
 * @brief   A class for momentary buttons and switches that send MIDI events.
 *
 * The button is debounced, and the button is latched (press once to enable, 
 * press again to disable) (toggle).
 *
 * @see     Button
 */
template <class Sender>
class MIDIButtonLatched : public MIDIOutputElement {
  protected:
    /**
     * @brief   Create a new MIDIButtonLatched object on the given pin and 
     *          address.
     * 
     * @param   pin
     *          The digital input pin to read from.  
     *          The internal pull-up resistor will be enabled.
     * @param   address
     *          The MIDI address containing the note number [0, 127], channel
     *          [1, 16], and optional cable number.
     */
    MIDIButtonLatched(pin_t pin, const MIDICNChannelAddress &address,
                      const Sender &sender)
        : button{pin}, address{address}, sender{sender} {}

  public:
    void begin() final override { button.begin(); }
    void update() final override {
        Button::State state = button.update();
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
        state ? sender.sendOn(address) : sender.sendOff(address);
    }

#ifdef INDIVIDUAL_BUTTON_INVERT
    void invert() { button.invert(); }
#endif

    Button::State getButtonState() const { return button.getState(); }

  private:
    Button button;
    const MIDICNChannelAddress address;
    Sender sender;
    bool state = false;
};
