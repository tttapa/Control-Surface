#pragma once

#include <MIDI_Outputs/Bankable/Abstract/MIDIRotaryEncoder.hpp>
#include <MIDI_Senders/RelativeCCSender.hpp>

namespace Bankable {

class CCRotaryEncoder : public MIDIRotaryEncoder<RelativeCCSender> {
  public:
    /**
     * @brief   Construct a new CCRotaryEncoder.
     *
     * @todo    Documentation
     */
    CCRotaryEncoder(const Encoder &encoder, uint8_t address, uint8_t channel,
                    uint8_t speedMultiply, uint8_t pulsesPerStep)
        : MIDIRotaryEncoder<RelativeCCSender>(encoder, address, channel,
                                              speedMultiply, pulsesPerStep) {}
};

} // namespace Bankable