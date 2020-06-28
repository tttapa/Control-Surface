#pragma once

#if not defined(Encoder_h_) && not defined(IDE)
#error                                                                         \
    "The PJRC Encoder library should be included before the Control-Surface "  \
    "library. (#include <Encoder.h>)"
#endif

#include <Banks/BankableAddresses.hpp>
#include <Def/Def.hpp>
#include <Encoder.h>
#include <MIDI_Outputs/Abstract/MIDIOutputElement.hpp>

BEGIN_CS_NAMESPACE

namespace Bankable {

/**
 * @brief   An abstract class for rotary encoders that send MIDI events and that
 *          can be added to a Bank.
 */
template <class BankAddress, class Sender>
class MIDIAbsoluteEncoder : public MIDIOutputElement {
  protected:
    /**
     * @brief   Construct a new MIDIAbsoluteEncoder.
     *
     * @todo    Documentation
     */
    MIDIAbsoluteEncoder(BankAddress bankAddress, 
                        const EncoderPinList &pins,
                         int16_t multiplier,
                        uint8_t pulsesPerStep, const Sender &sender)
        : address(bankAddress), encoder{pins.A, pins.B},
          multiplier(multiplier), pulsesPerStep(pulsesPerStep), sender(sender) {
    }

  public:
    void begin() final override {}
    void update() override {
        long currentPosition = getValue();
        if (currentPosition != previousPosition) {
            sender.send(currentPosition, address.getActiveAddress());
            previousPosition = currentPosition;
        }
    }

    analog_t getValue() {
        auto maxval = (1 << Sender::precision()) - 1;
        noInterrupts();
        auto rawval = encoder.read() * multiplier / pulsesPerStep;
        noInterrupts(); // encoder.read() enables interrupts :(
        auto val = constrain(rawval, 0, maxval);
        if (val != rawval)
            encoder.write(val * pulsesPerStep / multiplier);
        interrupts();
        return val;
    }

  private:
    BankAddress address;
    Encoder encoder;
    const int16_t multiplier;
    const uint8_t pulsesPerStep;
    long previousPosition = 0;

  public:
    Sender sender;
};

} // namespace Bankable

END_CS_NAMESPACE