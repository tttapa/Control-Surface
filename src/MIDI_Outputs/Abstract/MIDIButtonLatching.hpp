#pragma once

#include <AH/Hardware/Button.hpp>
#include <Def/Def.hpp>
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
     * @param   address
     *          The MIDI address to send to.
     * @param   sender
     *          The MIDI sender to use.
     */
    MIDIButtonLatching(pin_t pin, MIDIAddress address, const Sender &sender)
        : button(pin), address(address), sender(sender) {}

  public:
    void begin() override { button.begin(); }
    void update() override {
        AH::Button::State state = button.update();
        if (state == AH::Button::Falling || state == AH::Button::Rising) {
            sender.sendOn(address);
            sender.sendOff(address);
        }
    }

    AH::Button::State getButtonState() const { return button.getState(); }

    /// Get the MIDI address.
    MIDIAddress getAddress() const { return this->address; }
    /// Set the MIDI address.
    void setAddress(MIDIAddress address) { this->address = address; }

  private:
    AH::Button button;
    const MIDIAddress address;

  public:
    Sender sender;
};

END_CS_NAMESPACE