#pragma once

#include <stdint.h>

template <uint8_t BITS> class Hysteresis {
public:
  uint16_t getOutputLevel(uint16_t inputLevel);

private:
  uint8_t previousLevel = 0;
  const static uint8_t shiftFac = BITS;
  const static uint8_t margin = (1 << shiftFac) - 1;
};

// ------------ Implementation ------------ //

template <uint8_t BITS>
inline uint16_t Hysteresis<BITS>::getOutputLevel(uint16_t inputLevel) {
  uint16_t previousLevelFull = (uint16_t)previousLevel << shiftFac;
  uint16_t lowerbound = previousLevelFull;
  if (lowerbound > 0)
    lowerbound -= margin;
  uint16_t upperbound = previousLevelFull + margin;
  if (inputLevel < lowerbound || inputLevel > upperbound)
    previousLevel = inputLevel >> shiftFac;
  return previousLevel;
}