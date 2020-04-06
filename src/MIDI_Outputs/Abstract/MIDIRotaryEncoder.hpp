#pragma once

#include <AH/STL/type_traits> // std::make_signed
#include <AH/STL/utility> // std::forward
#include <Def/Def.hpp>
#include <MIDI_Outputs/Abstract/MIDIOutputElement.hpp>

AH_DIAGNOSTIC_WERROR()

BEGIN_CS_NAMESPACE

/**
 * @brief   An abstract class for rotary encoders that send MIDI events.
 */
template <class Enc, class Sender>
class GenericMIDIRotaryEncoder : public MIDIOutputElement {
  public:
    /**
     * @brief   Construct a new MIDIRotaryEncoder.
     *
     * @todo    Documentation
     */
    GenericMIDIRotaryEncoder(Enc &&encoder, MIDIAddress address,
                             int16_t speedMultiply, uint8_t pulsesPerStep,
                             const Sender &sender)
        : encoder(std::forward<Enc>(encoder)), address(address),
          speedMultiply(speedMultiply), pulsesPerStep(pulsesPerStep),
          sender(sender) {}

    void begin() override {}

    void update() override {
        Enc_t encval = encoder.read();
        // If Enc_t is an unsigned type, integer overflow is well-defined, which
        // is what we want when Enc_t is small and expected to overflow.
        // However, we need it to be signed because we're interrested  in the
        // delta.
        int16_t delta = SignedEnc_t(Enc_t(encval - deltaOffset));
        delta = delta * speedMultiply / pulsesPerStep;
        if (delta) {
            sender.send(delta, address);
            deltaOffset += Enc_t(delta * pulsesPerStep / speedMultiply);
        }
    }

  private:
    Enc encoder;
    MIDIAddress address;
    int16_t speedMultiply;
    uint8_t pulsesPerStep;
    using Enc_t = decltype(encoder.read());
    using SignedEnc_t = typename std::make_signed<Enc_t>::type;
    Enc_t deltaOffset = 0;

  public:
    Sender sender;
};

#if defined(Encoder_h_) || not defined(ARDUINO)

template <class Sender>
using MIDIRotaryEncoder = GenericMIDIRotaryEncoder<Encoder, Sender>;

template <class Sender>
using BorrowedMIDIRotaryEncoder = GenericMIDIRotaryEncoder<Encoder &, Sender>;

#endif

END_CS_NAMESPACE

AH_DIAGNOSTIC_POP()
