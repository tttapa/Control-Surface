#pragma once

#include "Channel.hpp"
#include "Frequency.hpp"
#include <Helpers/Array.hpp>
#include <stddef.h> // size_t
#include <stdint.h> // uint8_t

typedef int16_t analog_t;
typedef uint16_t pin_t;
constexpr pin_t NO_PIN = 1 << (8 * sizeof(pin_t) - 1);

using MappingFunction = int (*)(int);

template <class T, size_t nb_rows, size_t nb_cols>
using Array2D = Array<Array<T, nb_cols>, nb_rows>;

constexpr uint8_t NO_ADDRESS = {0x80};
template <uint8_t nb_rows, uint8_t nb_cols>
using AddressMatrix = Array2D<uint8_t, nb_rows, nb_cols>;

template <size_t N>
using PinList = Array<pin_t, N>;

using EncoderPinList = Array<uint8_t, 2>;
using EncoderSwitchPinList = Array<uint8_t, 3>;

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

struct PixelLocation {
    int16_t x;
    int16_t y;
};

#ifdef DOXYGEN
#define UNUSED_PARAM
#else
#define UNUSED_PARAM __attribute__((unused))
#endif