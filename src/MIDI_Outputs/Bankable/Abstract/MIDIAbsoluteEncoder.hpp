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
        Enc_t encval = encoder.read();
        // If Enc_t is an unsigned type, integer overflow is well-defined, which
        // is what we want when Enc_t is small and expected to overflow.
        // However, we need it to be signed because we're interested  in the
        // delta.
        int16_t delta = SignedEnc_t(Enc_t(encval - deltaOffset));
        delta = delta * multiplier / pulsesPerStep;
        if (delta) {
            address.lock();
            int16_t oldValue = values[address.getSelection()];
            int16_t newValue = oldValue + delta;
            newValue = constrain(newValue, 0, maxValue);
            if (oldValue != newValue) {
                sender.send(newValue, address.getActiveAddress());
                values[address.getSelection()] = newValue;
            }
            address.unlock();
            deltaOffset += Enc_t(delta * pulsesPerStep / multiplier);
        }
    }

    uint16_t getValue(setting_t bank) const { return values[bank]; }
    uint16_t getValue() const { return getValue(address.getSelection()); }

    void setValue(uint16_t value, setting_t bank) { values[bank] = value; }
    void setValue(uint16_t value) { setValue(value, address.getSelection()); }

  private:
    BankAddress address;
    Encoder encoder;
    const int16_t multiplier;
    const uint8_t pulsesPerStep;
    // long previousPosition = 0;
    Array<uint16_t, NumBanks> values = {{}};
    using Enc_t = decltype(encoder.read());
    using SignedEnc_t = typename std::make_signed<Enc_t>::type;
    Enc_t deltaOffset = 0;

    constexpr static int16_t maxValue = uint16_t(1u << Sender::precision()) - 1;

  public:
    Sender sender;
};

} // namespace Bankable

END_CS_NAMESPACE