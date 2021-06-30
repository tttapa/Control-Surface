#pragma once

#include "SevenSegmentDisplay.hpp"

BEGIN_CS_NAMESPACE

namespace MCU {

constexpr static uint8_t AssignmentDisplayLength = 2;
constexpr static uint8_t AssignmentDisplayAddress = 0x4A;

/// Class that receives and stores the text for the Mackie Control Universal
/// 7-segment assignment display.
///
/// @ingroup MIDIInputElements
class AssignmentDisplay : public SevenSegmentDisplay<AssignmentDisplayLength> {
  public:
    AssignmentDisplay(Channel channel = CHANNEL_1)
        : SevenSegmentDisplay<AssignmentDisplayLength>(
              {AssignmentDisplayAddress, channel}) {}
};

} // namespace MCU

END_CS_NAMESPACE