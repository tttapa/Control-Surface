#pragma once

#include <AH/STL/type_traits> // std::make_signed
#include <AH/STL/utility>     // std::forward
#include <Def/Def.hpp>
#include <Def/TypeTraits.hpp>
#include <MIDI_Outputs/Abstract/MIDIOutputElement.hpp>

#ifdef ARDUINO
#include <Submodules/Encoder/Encoder.h>
#else
#include <Encoder.h> // Mock
#endif

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

    void begin() override { begin_if_possible(encoder); }

    void update() override {
        Enc_t encval = encoder.read();
        // If Enc_t is an unsigned type, integer overflow is well-defined, which
        // is what we want when Enc_t is small and expected to overflow.
        // However, we need it to be signed because we're interested in the
        // delta.
        Enc_t uDelta = encval - deltaOffset;
        if (uDelta) {
            int16_t delta = SignedEnc_t(uDelta);
            // Assumption: delta and speedMultiply are relatively small, so
            // multiplication probably won't overflow.
            int16_t multipliedDelta = delta * speedMultiply + remainder;
            int16_t scaledDelta = multipliedDelta / pulsesPerStep;
            remainder = multipliedDelta % pulsesPerStep;

            if (scaledDelta)
                sender.send(scaledDelta, address);
            deltaOffset += uDelta;
        }
    }

  private:
    Enc encoder;
    MIDIAddress address;
    int16_t speedMultiply;
    uint8_t pulsesPerStep;
    int16_t remainder = 0;
    using Enc_t = decltype(encoder.read());
    using SignedEnc_t = typename std::make_signed<Enc_t>::type;
    Enc_t deltaOffset = 0;

  public:
    Sender sender;
};

template <class Sender>
using MIDIRotaryEncoder = GenericMIDIRotaryEncoder<Encoder, Sender>;

template <class Sender>
using BorrowedMIDIRotaryEncoder = GenericMIDIRotaryEncoder<Encoder &, Sender>;

END_CS_NAMESPACE

AH_DIAGNOSTIC_POP()
