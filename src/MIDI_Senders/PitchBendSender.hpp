#pragma once

#include <AH/Math/IncreaseBitDepth.hpp>
#include <Control_Surface/Control_Surface_Class.hpp>

BEGIN_CS_NAMESPACE

/**
 * @brief   Class that sends continuous MIDI pitch bend messages with a 
 *          resolution of 14 bits.
 * 
 * @tparam  INPUT_PRECISION_BITS
 *          The resolution of the input values. For example, if 
 *          @p INPUT_PRECISION_BITS == 10, the send function expects a @p value
 *          between 0 and 1023.
 * 
 * @ingroup MIDI_Senders
 */
template <uint8_t INPUT_PRECISION_BITS>
class PitchBendSender {
  public:
    /// Send a MIDI pitch bend message with the given value and channel+cable.
    /// address.getAddress() is ignored.
    /// Value should be @p INPUT_PRECISION_BITS wide.
    static void send(uint16_t value, MIDIAddress address) {
        value = AH::increaseBitDepth<14, precision(), uint16_t>(value);
        // ignore address byte, just use channel and cable numbers
        MIDIChannelCable channelCN = {address.getChannel(),
                                      address.getCableNumber()};
        Control_Surface.sendPitchBend(channelCN, value);
    }

    /// Get this sender's precision.
    constexpr static uint8_t precision() {
        static_assert(INPUT_PRECISION_BITS <= 14,
                      "Maximum pitch bend resolution is 14 bits");
        return INPUT_PRECISION_BITS;
    }
};

END_CS_NAMESPACE