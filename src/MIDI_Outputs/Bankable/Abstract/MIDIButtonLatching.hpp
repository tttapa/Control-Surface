#pragma once

#include <Banks/BankableMIDIOutput.hpp>
#include <Def/Def.hpp>
#include <Hardware/Button.hpp>
#include <MIDI_Outputs/Abstract/MIDIOutputElement.hpp>

BEGIN_CS_NAMESPACE

namespace Bankable {

/**
 * @brief   A class for latching buttons and switches that send MIDI events.
 *
 * The button is debounced.
 *
 * @see     Button
 */
template <class BankAddress, class Sender>
class MIDIButtonLatching : public MIDIOutputElement {
  protected:
    /**
     * @brief   Construct a new MIDIButtonLatching.
     *
     * @param   bankAddress
     *          The bankable MIDI address to send to.
     * @param   pin
     *          The digital input pin with the button connected.
     *          The internal pull-up resistor will be enabled.
     * @param   sender
     *          The MIDI sender to use.
     */
    MIDIButtonLatching(const BankAddress &bankAddress, pin_t pin,
                       const Sender &sender)
        : address{bankAddress}, button{pin}, sender{sender} {}

  public:
    void begin() final override { button.begin(); }
    void update() final override {
        Button::State state = button.getState();
        if (state == Button::Falling || state == Button::Rising) {
            MIDICNChannelAddress sendAddress = address.getActiveAddress();
            sender.sendOn(sendAddress);
            sender.sendOff(sendAddress);
        }
    }

  private:
    BankAddress address;
    Button button;

  public:
    Sender sender;
};

} // namespace Bankable

END_CS_NAMESPACE