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
                                  const MIDICNChannelAddress &address,
                                  uint8_t multiplier,
                                  const MIDICNChannelAddress &resetAddress,
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
            resetSender.sendOn(resetAddress);
            resetSender.sendOff(resetAddress);
        }
    }

#ifdef AH_INDIVIDUAL_BUTTON_INVERT
    void invert() { buttons.invert(); }
#endif

    AH::IncrementDecrementButtons::State getButtonsState() const {
        return buttons.getState();
    }

  private:
    AH::IncrementDecrementButtons buttons;
    const MIDICNChannelAddress address;
    const uint8_t multiplier;
    const MIDICNChannelAddress resetAddress;

  public:
    RelativeSender relativeSender;
    ResetSender resetSender;
};

END_CS_NAMESPACE