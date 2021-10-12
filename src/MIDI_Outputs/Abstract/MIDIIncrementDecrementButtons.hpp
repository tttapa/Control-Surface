#pragma once

#include <AH/Hardware/IncrementDecrementButtons.hpp>
#include <Def/Def.hpp>
#include <MIDI_Outputs/Abstract/MIDIOutputElement.hpp>

#include <MIDI_Senders/DigitalNoteSender.hpp>

BEGIN_CS_NAMESPACE

/**
 * @brief   An abstract class for two buttons that send incremental MIDI events.
 */
template <class RelativeSender, class ResetSender>
class MIDIIncrementDecrementButtons : public MIDIOutputElement {
  protected:
    /**
     * @brief   Construct a new MIDIIncrementDecrementButtons.
     *
     * @todo    Documentation
     */
    MIDIIncrementDecrementButtons(const AH::IncrementDecrementButtons &buttons,
                                  MIDIAddress address, uint8_t multiplier,
                                  MIDIAddress resetAddress,
                                  const RelativeSender &relativeSender,
                                  const ResetSender &resetSender)
        : buttons(buttons), address(address), multiplier(multiplier),
          resetAddress(resetAddress), relativeSender(relativeSender),
          resetSender(resetSender) {}

  public:
    void begin() override { buttons.begin(); }

    void update() override {
        using IncrDecrButtons = AH::IncrementDecrementButtons;
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
        if (resetAddress) {
            resetSender.sendOn(resetAddress);
            resetSender.sendOff(resetAddress);
        }
    }

    /// @see @ref AH::Button::invert()
    void invert() { buttons.invert(); }

    AH::IncrementDecrementButtons::State getButtonsState() const {
        return buttons.getState();
    }

  private:
    AH::IncrementDecrementButtons buttons;
    const MIDIAddress address;
    const uint8_t multiplier;
    const MIDIAddress resetAddress;

  public:
    RelativeSender relativeSender;
    ResetSender resetSender;
};

END_CS_NAMESPACE