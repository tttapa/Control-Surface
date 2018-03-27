#ifndef EMA_H_
#define EMA_H_

#include <stdint.h>

template <uint8_t shiftFac>
class EMA
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
template <uint8_t shiftFac>
class EMA64
{
  public:
    int64_t filter(int64_t value)
    {
        value = value << (shiftFac * 2);
        int64_t difference = value - filtered;
        filtered = filtered + (difference >> shiftFac);
        return (filtered + fixedPointAHalf) >> (shiftFac * 2);
    }

  private:
    int64_t filtered = 0;
    const static int64_t fixedPointAHalf = 1 << ((shiftFac * 2) - 1);
};

#endif // EMA_H_