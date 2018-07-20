#pragma once

#include "SevenSegmentDisplay.hpp"

namespace MCU {

constexpr static uint8_t AssignmentDisplayLength = 2;
constexpr static uint8_t AssignmentDisplayAddress = 0x4A;

class AssignmentDisplay
    : public SevenSegmentDisplay<AssignmentDisplayLength> {
  public:
    AssignmentDisplay(uint8_t channel = 1)
        : SevenSegmentDisplay<AssignmentDisplayLength>(
              channel, AssignmentDisplayAddress) {}
};

}