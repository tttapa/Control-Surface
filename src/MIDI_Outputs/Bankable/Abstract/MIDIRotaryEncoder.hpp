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
    GenericMIDIRotaryEncoder(BankAddress bankAddress, Enc &&encoder,
                             int16_t speedMultiply, uint8_t pulsesPerStep,
                             const Sender &sender)
        : address(bankAddress), encoder(std::forward<Enc>(encoder)),
          speedMultiply(speedMultiply), pulsesPerStep(pulsesPerStep),
          sender(sender) {}

  public:
    void begin() override {}

    void update() override {
        Enc_t encval = encoder.read();
        // If Enc_t is an unsigned type, integer overflow is well-defined, which
        // is what we want when Enc_t is small and expected to overflow.
        // However, we need it to be signed because we're interested  in the
        // delta.
        int16_t delta = SignedEnc_t(Enc_t(encval - deltaOffset));
        delta = delta * speedMultiply / pulsesPerStep;
        if (delta) {
            sender.send(delta, address.getActiveAddress());
            deltaOffset += Enc_t(delta * pulsesPerStep / speedMultiply);
        }
    }

  private:
    BankAddress address;
    Enc encoder;
    int16_t speedMultiply;
    uint8_t pulsesPerStep;
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
