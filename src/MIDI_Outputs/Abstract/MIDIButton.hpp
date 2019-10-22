#pragma once

#include <Def/Def.hpp>
#include <Hardware/Button.hpp>
#include <MIDI_Outputs/Abstract/MIDIOutputElement.hpp>

BEGIN_CS_NAMESPACE

/**
 * @brief   An abstract class for momentary push buttons that send MIDI events.
 *
 * The button is debounced.
 *
 * @see     Button
 */
template <class Sender>
class MIDIButton : public MIDIOutputElement {
  public:
    /**
     * @brief   Construct a new MIDIButton.
     *
     * @param   pin
     *          The digital input pin with the button connected.
     *          The internal pull-up resistor will be enabled.
     * @param   address
     *          The MIDI address to send to.
     * @param   sender
     *          The MIDI sender to use.
     */
    MIDIButton(pin_t pin, const MIDICNChannelAddress &address,
               const Sender &sender)
        : button{pin}, address{address}, sender{sender} {}

    void begin() override { button.begin(); }
    void update() override {
        Button::State state = button.getState();
        if (state == Button::Falling) {
            sender.sendOn(address);
        } else if (state == Button::Rising) {
            sender.sendOff(address);
        }
    }

#ifdef INDIVIDUAL_BUTTON_INVERT
    void invert() { button.invert(); }
#endif

    Button getButtonCopy() const { return button; }

  private:
    Button button;
    const MIDICNChannelAddress address;

  public:
    Sender sender;
};

END_CS_NAMESPACE