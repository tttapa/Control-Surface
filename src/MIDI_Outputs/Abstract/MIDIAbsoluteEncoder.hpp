#pragma once

#include <AH/STL/type_traits> // std::make_signed
#include <AH/STL/utility> // std::forward
#include <Def/Def.hpp>
#include <Encoder.h>
#include <MIDI_Outputs/Abstract/MIDIOutputElement.hpp>

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
                               int16_t multiplier, uint8_t pulsesPerStep,
                               const Sender &sender)
        : encoder(std::forward<Enc>(encoder)), address(address),
          multiplier(multiplier), pulsesPerStep(pulsesPerStep), sender(sender) {
    }

    void begin() override {}

    void update() override {
        Enc_t encval = encoder.read();
        // If Enc_t is an unsigned type, integer overflow is well-defined, which
        // is what we want when Enc_t is small and expected to overflow.
        // However, we need it to be signed because we're interrested  in the
        // delta.
        int16_t delta = SignedEnc_t(Enc_t(encval - deltaOffset));
        delta = delta * multiplier / pulsesPerStep;
        if (delta) {
            int16_t oldValue = value;
            int16_t newValue = oldValue + delta;
            newValue = constrain(newValue, 0, maxValue);
            if (oldValue != newValue) {
                sender.send(newValue, address);
                value = newValue;
            }
            deltaOffset += Enc_t(delta * pulsesPerStep / multiplier);
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

  private:
    Enc encoder;
    MIDIAddress address;
    int16_t multiplier;
    uint8_t pulsesPerStep;
    uint16_t value = 0;
    using Enc_t = decltype(encoder.read());
    using SignedEnc_t = typename std::make_signed<Enc_t>::type;
    Enc_t deltaOffset = 0;

    constexpr static uint16_t maxValue = (1 << Sender::precision()) - 1;

  public:
    Sender sender;
};

#if defined(Encoder_h_) || not defined(ARDUINO)

template <class Sender>
using MIDIAbsoluteEncoder = GenericMIDIAbsoluteEncoder<Encoder, Sender>;

template <class Sender>
using BorrowedMIDIAbsoluteEncoder =
    GenericMIDIAbsoluteEncoder<Encoder &, Sender>;

#endif

END_CS_NAMESPACE

AH_DIAGNOSTIC_POP()
