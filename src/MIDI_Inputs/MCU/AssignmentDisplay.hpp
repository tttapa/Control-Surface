#pragma once

#include "SevenSegmentDisplay.hpp"

namespace MCU {

constexpr static uint8_t AssignmentDisplayLength = 2;
constexpr static uint8_t AssignmentDisplayAddress = 0x4A;

class AssignmentDisplay : public SevenSegmentDisplay<AssignmentDisplayLength> {
  public:
    AssignmentDisplay(Channel channel = CHANNEL_1)
        : SevenSegmentDisplay<AssignmentDisplayLength>(
              {AssignmentDisplayAddress, channel}) {}
};

} // namespace MCU