#pragma once

#if not defined(Encoder_h_) && not defined(IDE)
#error                                                                         \
    "The PJRC Encoder library should be included before the Control-Surface "  \
    "library. (#include <Encoder.h>)"
#endif

#include <AH/STL/utility> // std::forward
#include <Def/Def.hpp>
#include <Encoder.h>
#include <MIDI_Outputs/Abstract/MIDIOutputElement.hpp>

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
    GenericMIDIRotaryEncoder(Enc &&encoder, const MIDICNChannelAddress &address,
                             int8_t speedMultiply, uint8_t pulsesPerStep,
                             const Sender &sender)
        : encoder(std::forward<Enc>(encoder)), address(address),
          speedMultiply(speedMultiply), pulsesPerStep(pulsesPerStep),
          sender(sender) {}

// For tests only
#ifndef ARDUINO
    GenericMIDIRotaryEncoder(const Encoder &encoder,
                             const MIDICNChannelAddress &address,
                             int8_t speedMultiply, uint8_t pulsesPerStep,
                             const Sender &sender)
        : encoder{encoder}, address(address), speedMultiply(speedMultiply),
          pulsesPerStep(pulsesPerStep), sender(sender) {}
#endif

    void begin() final override {}
    void update() final override {
        long currentPosition = encoder.read();
        long difference = (currentPosition - previousPosition) / pulsesPerStep;
        if (difference) {
            sender.send(difference * speedMultiply, address);
            previousPosition += difference * pulsesPerStep;
        }
    }

    void resetPositionOffset() { previousPosition = encoder.read(); }

  private:
    Enc encoder;
    const MIDICNChannelAddress address;
    const int8_t speedMultiply;
    const uint8_t pulsesPerStep;
    long previousPosition = 0;

  public:
    Sender sender;
};

template <class Sender>
using MIDIRotaryEncoder = GenericMIDIRotaryEncoder<Encoder, Sender>;

template <class Sender>
using BorrowedMIDIRotaryEncoder = GenericMIDIRotaryEncoder<Encoder &, Sender>;

END_CS_NAMESPACE