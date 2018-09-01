#pragma once

#include <Helpers/Array.hpp>
#include <stddef.h> // size_t
#include <stdint.h> // uint8_t

typedef uint16_t pin_t;
typedef int16_t analog_t;

constexpr pin_t NO_PIN = {0x8000};

using MappingFunction = int (*)(int);

constexpr uint8_t NO_ADDRESS = {0x80};

template <size_t N>
using PinList = Array<pin_t, N>;

using EncoderPinList = Array<uint8_t, 2>;
using EncoderSwitchPinList = Array<uint8_t, 3>;

template <class T, size_t nb_rows, size_t nb_cols>
using Array2D = Array<Array<T, nb_cols>, nb_rows>;

// channel, address
using DigitalSendFunction = void (*)(uint8_t, uint8_t);
// value, channel, address
using ContinuousSendFunction7Bit = void (*)(uint8_t, uint8_t, uint8_t);
// value, channel
using ContinuousSendFunction14Bit = void (*)(uint16_t, uint8_t);
// delta, channel, address
using RelativeSendFunction = void (*)(long, uint8_t, uint8_t);

using setting_t = uint8_t;

// Updatable types:
struct NormalUpdatable {};
struct Potentiometer {};
struct MotorFader {};
struct Display {};

// Frequency

class Frequency {
  public:
    explicit constexpr Frequency(unsigned long hertz) : hertz(hertz) {}
    operator unsigned long() const { return hertz; }

  private:
    unsigned long hertz;
};
constexpr Frequency operator"" _Hz(unsigned long long hz) {
    return Frequency{(unsigned long)hz};
}
constexpr Frequency operator"" _kHz(long double khz) {
    return Frequency{(unsigned long)(khz * 1E3)};
}
constexpr Frequency operator"" _kHz(unsigned long long khz) {
    return Frequency{(unsigned long)(khz * 1E3)};
}
constexpr Frequency operator"" _MHz(long double mhz) {
    return Frequency{(unsigned long)(mhz * 1E6)};
}
constexpr Frequency operator"" _MHz(unsigned long long mhz) {
    return Frequency{(unsigned long)(mhz * 1E6)};
}

#ifdef DOXYGEN
#define UNUSED_PARAM
#else
#define UNUSED_PARAM __attribute__((unused))
#endif