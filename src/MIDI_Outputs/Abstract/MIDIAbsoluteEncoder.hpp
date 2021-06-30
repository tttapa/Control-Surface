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
 * @brief   An abstract class for rotary encoders that send absolute MIDI 
 *          events.
 */
template <class Enc, class Sender>
class GenericMIDIAbsoluteEncoder : public MIDIOutputElement {
  public:
    GenericMIDIAbsoluteEncoder(Enc &&encoder, MIDIAddress address,
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

            int16_t oldValue = value;
            int16_t newValue = oldValue + scaledDelta;
            newValue = constrain(newValue, 0, maxValue);
            if (oldValue != newValue) {
                sender.send(newValue, address);
                value = newValue;
            }

            deltaOffset += uDelta;
        }
    }

    /**
     * @brief   Get the absolute value of the encoder.
     */
    uint16_t getValue() const { return value; }

    /**
     * @brief   Set the absolute value of the encoder.
     */
    void setValue(uint16_t value) { this->value = value; }

    void setSpeedMultiply(int16_t speedMultiply) {
        // TODO: Is this correct? Is it necessary? What with negative speedMult?
        remainder = remainder * speedMultiply / this->speedMultiply;
        this->speedMultiply = speedMultiply;
    }
    int16_t getSpeedMultiply() const { return this->speedMultiply; }

  private:
    Enc encoder;
    MIDIAddress address;
    int16_t speedMultiply;
    uint8_t pulsesPerStep;
    int16_t value = 0;
    int16_t remainder = 0;
    using Enc_t = decltype(encoder.read());
    using SignedEnc_t = typename std::make_signed<Enc_t>::type;
    Enc_t deltaOffset = 0;

    constexpr static int16_t maxValue = uint16_t(1u << Sender::precision()) - 1;

  public:
    Sender sender;
};

template <class Sender>
using MIDIAbsoluteEncoder = GenericMIDIAbsoluteEncoder<Encoder, Sender>;

template <class Sender>
using BorrowedMIDIAbsoluteEncoder =
    GenericMIDIAbsoluteEncoder<Encoder &, Sender>;

END_CS_NAMESPACE

AH_DIAGNOSTIC_POP()
