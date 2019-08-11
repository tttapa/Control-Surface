#pragma once

#include <Banks/BankableMIDIOutput.hpp>
#include <Def/Def.hpp>
#include <Hardware/Button.hpp>
#include <MIDI_Outputs/Abstract/MIDIOutputElement.hpp>

namespace Bankable {

/**
 * @brief   An abstract class for momentary push buttons that send MIDI events.
 *
 * The button is debounced.
 *
 * @see     Button
 */
template <class Sender>
class MIDIButton : public BankableMIDIOutput, public MIDIOutputElement {
  public:
    /**
     * @brief   Construct a new bankable MIDIButton.
     *
     * @param   pin
     *          The digital input pin with the button connected.
     *          The internal pull-up resistor will be enabled.
     */
    MIDIButton(const OutputBankConfig &config, pin_t pin,
               const MIDICNChannelAddress &address, const Sender &sender)
        : BankableMIDIOutput{config}, button{pin}, address{address},
          sender{sender} {}

    void begin() final override { button.begin(); }
    void update() final override {
        Button::State state = button.update();
        MIDICNChannelAddress sendAddress = address;
        if (state == Button::Falling) {
            lock();
            sendAddress += getAddressOffset();
            sender.sendOn(sendAddress);
        } else if (state == Button::Rising) {
            sendAddress += getAddressOffset();
            sender.sendOff(sendAddress);
            unlock();
        }
    }

#ifdef INDIVIDUAL_BUTTON_INVERT
    void invert() { button.invert(); }
#endif

    Button::State getButtonState() const { return button.getState(); }

  private:
    Button button;
    const MIDICNChannelAddress address;
    Sender sender;
};

} // namespace Bankable