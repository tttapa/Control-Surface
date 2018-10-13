#pragma once

#include <Helpers/Array.hpp>
#include <stddef.h> // size_t
#include <stdint.h> // uint8_t

typedef int16_t analog_t;
typedef uint16_t pin_t;
constexpr pin_t NO_PIN = 1 << (8 * sizeof(pin_t) - 1);

using MappingFunction = int (*)(int);

constexpr uint8_t NO_ADDRESS = {0x80};

template <size_t N>
using PinList = Array<pin_t, N>;

using EncoderPinList = Array<uint8_t, 2>;
using EncoderSwitchPinList = Array<uint8_t, 3>;

template <class T, size_t nb_rows, size_t nb_cols>
using Array2D = Array<Array<T, nb_cols>, nb_rows>;

class MIDICNChannelAddress;

// channel, address
using DigitalSendFunction = void (*)(MIDICNChannelAddress);
// value, channel, address
using ContinuousSendFunction7Bit = void (*)(uint8_t, MIDICNChannelAddress);
// value, channel
using ContinuousSendFunction14Bit = void (*)(uint16_t, MIDICNChannelAddress);
// delta, channel, address
using RelativeSendFunction = void (*)(long, MIDICNChannelAddress);

using setting_t = uint8_t;
constexpr setting_t NO_SETTING = 1 << (8 * sizeof(setting_t) - 1);

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

struct PixelLocation {
    int16_t x;
    int16_t y;
};

class Channel {
  public:
    explicit constexpr Channel(int8_t zeroBasedChannel)
        : zeroBasedChannel(zeroBasedChannel) {}

    constexpr operator int8_t() const { return zeroBasedChannel; }

    static constexpr Channel createChannel(int8_t oneBasedChannel) {
        return Channel{int8_t(oneBasedChannel - 1)};
    }

  private:
    int8_t zeroBasedChannel;
};

constexpr Channel operator"" _ch(unsigned long long ch) {
    return Channel::createChannel(ch);
}

constexpr Channel CHANNEL_1 = 1_ch;
constexpr Channel CHANNEL_2 = 2_ch;
constexpr Channel CHANNEL_3 = 3_ch;
constexpr Channel CHANNEL_4 = 4_ch;
constexpr Channel CHANNEL_5 = 5_ch;
constexpr Channel CHANNEL_6 = 6_ch;
constexpr Channel CHANNEL_7 = 7_ch;
constexpr Channel CHANNEL_8 = 8_ch;
constexpr Channel CHANNEL_9 = 9_ch;
constexpr Channel CHANNEL_10 = 10_ch;
constexpr Channel CHANNEL_11 = 11_ch;
constexpr Channel CHANNEL_12 = 12_ch;
constexpr Channel CHANNEL_13 = 13_ch;
constexpr Channel CHANNEL_14 = 14_ch;
constexpr Channel CHANNEL_15 = 15_ch;
constexpr Channel CHANNEL_16 = 16_ch;

#ifdef DOXYGEN
#define UNUSED_PARAM
#else
#define UNUSED_PARAM __attribute__((unused))
#endif