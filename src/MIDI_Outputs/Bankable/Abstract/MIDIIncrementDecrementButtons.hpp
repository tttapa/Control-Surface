#pragma once

#include <Banks/BankableMIDIOutput.hpp>
#include <Def/Def.hpp>
#include <Hardware/IncrementDecrementButtons.hpp>
#include <MIDI_Outputs/Abstract/MIDIOutputElement.hpp>

#include <MIDI_Senders/DigitalNoteSender.hpp>

namespace Bankable {

/**
 * @brief   An abstract class for two buttons that send incremental MIDI events.
 * 
 * @todo    Combine RelativeSender and ResetSender?
 */
template <class RelativeSender, class ResetSender>
class MIDIIncrementDecrementButtons : public BankableMIDIOutput,
                                      public MIDIOutputElement {
  protected:
    /**
     * @brief   Construct a new MIDIIncrementDecrementButtons.
     *
     * @todo    Documentation
     */
    MIDIIncrementDecrementButtons(const OutputBankConfig &config,
                                  const IncrementDecrementButtons &buttons,
                                  const MIDICNChannelAddress &address,
                                  uint8_t multiplier,
                                  const MIDICNChannelAddress &resetAddress,
                                  const RelativeSender &relativeSender,
                                  const ResetSender &resetSender)
        : BankableMIDIOutput{config}, buttons(buttons), address(address),
          multiplier(multiplier), resetAddress(resetAddress),
          relativeSender{relativeSender}, resetSender{resetSender} {}

  public:
    void begin() override { buttons.begin(); }

    void update() override {
        using IncrDecrButtons = IncrementDecrementButtons;
        MIDICNChannelAddress address = this->address + getAddressOffset();
        switch (buttons.update()) {
            case IncrDecrButtons::Increment: send(multiplier, address); break;
            case IncrDecrButtons::Decrement: send(-multiplier, address); break;
            case IncrDecrButtons::Reset: reset(); break;
            case IncrDecrButtons::Nothing: break;
            default: break;
        }
    }

    void send(long delta, const MIDICNChannelAddress &address) {
        relativeSender.send(delta, address);
    }

    void reset() {
        if (resetAddress) {
            MIDICNChannelAddress address = resetAddress + getAddressOffset();
            resetSender.sendOn(address);
            resetSender.sendOff(address);
        }
    }

#ifdef INDIVIDUAL_BUTTON_INVERT
    void invert() { buttons.invert(); }
#endif

    IncrementDecrementButtons::State getButtonsState() const {
        return buttons.getState();
    }

  private:
    IncrementDecrementButtons buttons;
    const MIDICNChannelAddress address;
    const uint8_t multiplier;
    const MIDICNChannelAddress resetAddress;
    RelativeSender relativeSender;
    ResetSender resetSender;
};

} // namespace Bankable