#pragma once

#include <AH/Hardware/IncrementDecrementButtons.hpp>
#include <Banks/BankableAddresses.hpp>
#include <Def/Def.hpp>
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
    MIDIIncrementDecrementButtons(BankAddress addresses,
                                  const AH::IncrementDecrementButtons &buttons,
                                  uint8_t multiplier,
                                  const RelativeSender &relativeSender,
                                  const ResetSender &resetSender)
        : addresses(addresses), buttons(buttons), multiplier(multiplier),
          relativeSender(relativeSender), resetSender(resetSender) {}

  public:
    void begin() override { buttons.begin(); }

    void update() override {
        using IncrDecrButtons = AH::IncrementDecrementButtons;
        MIDIAddress address = addresses.getFirstActiveAddress();
        switch (buttons.update()) {
            case IncrDecrButtons::Nothing: break;
            case IncrDecrButtons::IncrementShort: // fallthrough
            case IncrDecrButtons::IncrementLong:  // fallthrough
            case IncrDecrButtons::IncrementHold:
                send(multiplier, address);
                break;
            case IncrDecrButtons::DecrementShort: // fallthrough
            case IncrDecrButtons::DecrementLong:  // fallthrough
            case IncrDecrButtons::DecrementHold:
                send(-multiplier, address);
                break;
            case IncrDecrButtons::Reset: reset(); break;
            default: break;
        }
    }

    void send(long delta, MIDIAddress address) {
        relativeSender.send(delta, address);
    }

    void reset() {
        MIDIAddress address = addresses.getSecondActiveAddress();
        if (address) {
            resetSender.sendOn(address);
            resetSender.sendOff(address);
        }
    }

    /// @see @ref AH::Button::invert()
    void invert() { buttons.invert(); }

    AH::IncrementDecrementButtons::State getButtonsState() const {
        return buttons.getState();
    }

  protected:
    BankAddress addresses;
    AH::IncrementDecrementButtons buttons;
    const uint8_t multiplier;

  public:
    RelativeSender relativeSender;
    ResetSender resetSender;
};

} // namespace Bankable

END_CS_NAMESPACE