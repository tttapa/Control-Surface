#pragma once

#if not defined(Encoder_h_) && not defined(IDE)
#error                                                                         \
    "The PJRC Encoder library should be included before the Control-Surface "  \
    "library. (#include <Encoder.h>)"
#endif

#include <Def/Def.hpp>
#include <Encoder.h>
#include <MIDI_Outputs/Abstract/MIDIOutputElement.hpp>

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
        int32_t encval = encoder.read();
        int32_t delta = (encval - deltaOffset) * multiplier / pulsesPerStep;
        if (delta) {
            uint16_t oldValue = value;
            int32_t newValue = oldValue + delta;
            newValue = constrain(newValue, 0, maxValue);
            if (oldValue != newValue) {
                sender.send(newValue, address);
                value = newValue;
            }
            deltaOffset += delta * pulsesPerStep / multiplier;
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
    long deltaOffset = 0;

    constexpr static uint16_t maxValue = (1 << Sender::precision()) - 1;

  public:
    Sender sender;
};

template <class Sender>
using MIDIAbsoluteEncoder = GenericMIDIAbsoluteEncoder<Encoder, Sender>;

template <class Sender>
using BorrowedMIDIAbsoluteEncoder =
    GenericMIDIAbsoluteEncoder<Encoder &, Sender>;

END_CS_NAMESPACE