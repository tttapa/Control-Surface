#include "./Hysteresis.h"

uint16_t Hysteresis::getOutputLevel(uint16_t inputLevel) {
  uint16_t previousLevelFull = (uint16_t) previousLevel << shiftFac;
  uint16_t lb = previousLevelFull;
  if (lb > 0)
    lb -= margin;
  uint16_t ub = previousLevelFull + margin;
  if ( inputLevel < lb || inputLevel > ub)
    previousLevel = inputLevel >> shiftFac;
  return previousLevel;
}
