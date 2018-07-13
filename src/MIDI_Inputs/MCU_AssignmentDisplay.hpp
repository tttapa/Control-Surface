#pragma once

#include "MCU_7SegmentDisplay.hpp"

constexpr static uint8_t AssignmentDisplayLength = 2;
constexpr static uint8_t AssignmentDisplayAddress = 0x4A;

class MCU_AssignmentDisplay
    : public MCU_7SegmentDisplay<AssignmentDisplayLength> {
  public:
    MCU_AssignmentDisplay(uint8_t channel = 1)
        : MCU_7SegmentDisplay<AssignmentDisplayLength>(
              channel, AssignmentDisplayAddress) {}
};