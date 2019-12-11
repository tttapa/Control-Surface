#pragma once

#include <Banks/BankableMIDIOutput.hpp>
#include <Def/Def.hpp>
#include <AH/Hardware/IncrementDecrementButtons.hpp>
#include <MIDI_Outputs/Abstract/MIDIOutputElement.hpp>

#include <MIDI_Senders/DigitalNoteSender.hpp>

BEGIN_CS_NAMESPACE

namespace Bankable {

/**
 * @brief   An abstract class for two buttons that send incremental MIDI events.
 * 
 * @todo    Combine RelativeSender and ResetSender?
 */
template <class BankAddress, class RelativeSender, class ResetSender>
class MIDIIncrementDecrementButtons : public MIDIOutputElement {
  protected:
    /**
     * @brief   Construct a new MIDIIncrementDecrementButtons.
     *
     * @todo    Documentation
     */
    MIDIIncrementDecrementButtons(const BankAddress &addresses,
                                  const AH::IncrementDecrementButtons &buttons,
                                  uint8_t multiplier,
                                  const RelativeSender &relativeSender,
                                  const ResetSender &resetSender)
        : addresses{addresses}, buttons{buttons}, multiplier{multiplier},
          relativeSender(relativeSender), resetSender(resetSender) {}

  public:
    void begin() override { buttons.begin(); }

    void update() override {
        using IncrDecrButtons = AH::IncrementDecrementButtons;
        MIDICNChannelAddress address = addresses.getActiveAddress(0);
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
        MIDICNChannelAddress address = addresses.getActiveAddress(1);
        if (address) {
            resetSender.sendOn(address);
            resetSender.sendOff(address);
        }
    }

#ifdef AH_INDIVIDUAL_BUTTON_INVERT
    void invert() { buttons.invert(); }
#endif

    AH::IncrementDecrementButtons::State getButtonsState() const {
        return buttons.getState();
    }

  private:
    BankAddress addresses;
    AH::IncrementDecrementButtons buttons;
    const uint8_t multiplier;

  public:
    RelativeSender relativeSender;
    ResetSender resetSender;
};

} // namespace Bankable

END_CS_NAMESPACE