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
template <DigitalSendFunction sendOn, DigitalSendFunction sendOff>
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
                       const MIDICNChannelAddress &address)
        : BankableMIDIOutput(config), button{pin}, address(address) {}

  public:
    void begin() final override { button.begin(); }
    void update() final override {
        Button::State state = button.getState();
        MIDICNChannelAddress sendAddress = address;
        sendAddress += getAddressOffset();
        if (state == Button::Falling || state == Button::Rising) {
            sendOn(sendAddress);
            sendOff(sendAddress);
        }
    }

  private:
    Button button;
    const MIDICNChannelAddress address;
};

} // namespace Bankable