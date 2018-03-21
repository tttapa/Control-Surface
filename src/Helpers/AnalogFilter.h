#ifndef ANALOG_FILTER_H_
#define ANALOG_FILTER_H_

#include <cstdint>

template <uint8_t shiftFac>
class AnalogFilter
{
  public:
    int32_t filter(int32_t value)
    {
        value = value << (shiftFac * 2);
        int32_t difference = value - filtered;
        filtered = filtered + (difference >> shiftFac);
        return (filtered + fixedPointAHalf) >> (shiftFac * 2);
    }

  private:
    int32_t filtered = 0;
    const static int32_t fixedPointAHalf = 1 << ((shiftFac * 2) - 1);
};

#endif // ANALOG_FILTER_H_