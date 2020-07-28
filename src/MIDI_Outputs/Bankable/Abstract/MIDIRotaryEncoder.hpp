#pragma once

#include <AH/STL/type_traits> // std::make_signed
#include <AH/STL/utility>     // std::forward
#include <Banks/BankableAddresses.hpp>
#include <Def/Def.hpp>
#include <MIDI_Outputs/Abstract/MIDIOutputElement.hpp>

AH_DIAGNOSTIC_WERROR()

BEGIN_CS_NAMESPACE

namespace Bankable {

/**
 * @brief   An abstract class for rotary encoders that send MIDI events and that
 *          can be added to a Bank.
 */
template <class Enc, class BankAddress, class Sender>
class GenericMIDIRotaryEncoder : public MIDIOutputElement {
  protected:
    /**
     * @brief   Construct a new GenericMIDIRotaryEncoder.
     *
     * @todo    Documentation
     */
    GenericMIDIRotaryEncoder(const BankAddress &bankAddress, Enc &&encoder,
                             int16_t multiplier, uint8_t pulsesPerStep,
                             const Sender &sender)
        : address(bankAddress), encoder(std::forward<Enc>(encoder)),
          multiplier(multiplier), pulsesPerStep(pulsesPerStep), sender(sender) {
    }

  public:
    void begin() override {}

    void update() override {
        Enc_t encval = encoder.read();
        // If Enc_t is an unsigned type, integer overflow is well-defined, which
        // is what we want when Enc_t is small and expected to overflow.
        // However, we need it to be signed because we're interested in the
        // delta.
        Enc_t uDelta = encval - deltaOffset;
        if (uDelta) {
            int16_t delta = SignedEnc_t(uDelta);
            // Assumption: delta and multiplier are relatively small, so
            // multiplication probably won't overflow.
            int16_t multipliedDelta = delta * multiplier + remainder;
            int16_t scaledDelta = multipliedDelta / pulsesPerStep;
            remainder = multipliedDelta % pulsesPerStep;

            // No locking of the address necessary
            if (scaledDelta)
                sender.send(scaledDelta, address.getActiveAddress());
            deltaOffset += uDelta;
        }
    }

  private:
    BankAddress address;
    Enc encoder;
    int16_t multiplier;
    uint8_t pulsesPerStep;
    int16_t remainder = 0;
    using Enc_t = decltype(encoder.read());
    using SignedEnc_t = typename std::make_signed<Enc_t>::type;
    Enc_t deltaOffset = 0;

  public:
    Sender sender;
};

#if defined(Encoder_h_) || not defined(ARDUINO)

template <class BankAddress, class Sender>
using MIDIRotaryEncoder =
    GenericMIDIRotaryEncoder<Encoder, BankAddress, Sender>;

template <class BankAddress, class Sender>
using BorrowedMIDIRotaryEncoder =
    GenericMIDIRotaryEncoder<Encoder &, BankAddress, Sender>;

#endif

} // namespace Bankable

END_CS_NAMESPACE

AH_DIAGNOSTIC_POP()
