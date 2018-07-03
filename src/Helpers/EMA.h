#ifndef EMA_H_
#define EMA_H_

#include <stdint.h>

template <uint8_t shiftFac, class int_t>
class EMA {
  public:
    int_t filter(int_t value);

  private:
    int_t filtered = 0;
    const static int_t fixedPointAHalf = 1 << ((shiftFac * 2) - 1);
};

// ------------ Implementation ------------ //

template <uint8_t shiftFac, class int_t>
int_t EMA<shiftFac, int_t>::filter(int_t value) {
    value = value << (shiftFac * 2);
    int_t difference = value - filtered;
    filtered = filtered + (difference >> shiftFac);
    return (filtered + fixedPointAHalf) >> (shiftFac * 2);
}

#endif // EMA_H_