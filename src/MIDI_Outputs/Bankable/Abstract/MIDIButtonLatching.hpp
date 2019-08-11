#pragma once

#include <Banks/BankableMIDIOutput.hpp>
#include <Def/Def.hpp>
#include <Hardware/Button.hpp>
#include <MIDI_Outputs/Abstract/MIDIOutputElement.hpp>

namespace Bankable {

/**
 * @brief   A class for latching buttons and switches that send MIDI events.
 *
 * The button is debounced.
 *
 * @see     Button
 */
template <class Sender>
class MIDIButtonLatching : public MIDIOutputElement, public BankableMIDIOutput {
  protected:
    /**
     * @brief   Construct a new MIDIButtonLatching.
     *
     * @param   pin
     *          The digital input pin with the button connected.
     *          The internal pull-up resistor will be enabled.
     */
    MIDIButtonLatching(const OutputBankConfig &config, pin_t pin,
                       const MIDICNChannelAddress &address,
                       const Sender &sender)
        : BankableMIDIOutput{config}, button{pin}, address{address},
          sender{sender} {}

  public:
    void begin() final override { button.begin(); }
    void update() final override {
        Button::State state = button.update();
        MIDICNChannelAddress sendAddress = address;
        sendAddress += getAddressOffset();
        if (state == Button::Falling || state == Button::Rising) {
            sender.sendOn(sendAddress);
            sender.sendOff(sendAddress);
        }
    }

    Button::State getButtonState() const { return button.getState(); }

  private:
    Button button;
    const MIDICNChannelAddress address;
    Sender sender;
};

} // namespace Bankable