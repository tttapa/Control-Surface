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
    GenericMIDIAbsoluteEncoder(Enc &&encoder, MIDICNChannelAddress address,
                               int16_t multiplier, uint8_t pulsesPerStep,
                               const Sender &sender)
        : encoder(std::forward<Enc>(encoder)), address(address),
          multiplier(multiplier), pulsesPerStep(pulsesPerStep), sender(sender) {
    }

    void begin() override { resetPositionOffset(); }
    void update() override {
        uint16_t currentPosition = getNewValue();
        if (currentPosition != previousPosition) {
            sender.send(currentPosition, address);
            previousPosition = currentPosition;
        }
    }

    uint16_t getNewValue() {
        auto maxval = (1 << Sender::precision()) - 1;
        auto encval = encoder.read();
        long rawval =
            (encval - zeroOffsetPosition) * multiplier / pulsesPerStep;
        auto val = constrain(rawval, 0, maxval);
        if (val != rawval)
            zeroOffsetPosition =
                encval + (rawval - val) * pulsesPerStep / multiplier;
        return val;
    }

    uint16_t getValue() const { return previousPosition; }

    void resetPositionOffset() { set(getValue()); }

    void set(uint16_t value) {
        zeroOffsetPosition =
            encoder.read() - (long)value * pulsesPerStep / multiplier;
    }

  private:
    Enc encoder;
    MIDICNChannelAddress address;
    int16_t multiplier;
    uint8_t pulsesPerStep;
    uint16_t previousPosition = 0;
    long zeroOffsetPosition = 0;

  public:
    Sender sender;
};

template <class Sender>
using MIDIAbsoluteEncoder = GenericMIDIAbsoluteEncoder<Encoder, Sender>;

template <class Sender>
using BorrowedMIDIAbsoluteEncoder =
    GenericMIDIAbsoluteEncoder<Encoder &, Sender>;

END_CS_NAMESPACE