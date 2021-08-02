#pragma once

#include <AH/Hardware/Button.hpp>
#include <Def/Def.hpp>
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
    MIDIButton(pin_t pin, MIDIAddress address, const Sender &sender)
        : button(pin), address(address), sender(sender) {}

    void begin() override { button.begin(); }
    void update() override {
        AH::Button::State state = button.update();
        if (state == AH::Button::Falling) {
            sender.sendOn(address);
        } else if (state == AH::Button::Rising) {
            sender.sendOff(address);
        }
    }

    /// @see @ref AH::Button::invert()
    void invert() { button.invert(); }

    AH::Button::State getButtonState() const { return button.getState(); }

  private:
    AH::Button button;
    const MIDIAddress address;

  public:
    Sender sender;
};

END_CS_NAMESPACE