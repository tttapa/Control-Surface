#ifndef HystFilter_h
#define HystFilter_h

#include <stdint.h>

class Hysteresis
{
  public:
    uint16_t getOutputLevel(uint16_t inputLevel);

  private:
    uint8_t previousLevel = 0;
    const static uint8_t shiftFac = 3;
    const static uint8_t margin = (1 << shiftFac) - 1;
};

#endif