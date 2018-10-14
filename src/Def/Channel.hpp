#pragma once

#include <stdint.h> // int8_t

class Channel {
  public:
    explicit constexpr Channel(int8_t zeroBasedChannel)
        : zeroBasedChannel(zeroBasedChannel) {}

    constexpr int8_t getRaw() const { return zeroBasedChannel; }

    static constexpr Channel createChannel(int8_t oneBasedChannel) {
        return Channel{int8_t(oneBasedChannel - 1)};
    }

    constexpr bool operator==(const Channel &rhs) const {
        return this->zeroBasedChannel == rhs.zeroBasedChannel;
    }

    Channel &operator+=(const int8_t rhs) {
        this->zeroBasedChannel += rhs;
        return *this;
    }

    Channel operator+(const int8_t rhs) const {
        Channel copy = *this;
        copy += rhs;
        return copy;
    }

    Channel &operator-=(const int8_t rhs) {
        this->zeroBasedChannel -= rhs;
        return *this;
    }

    Channel operator-(const int8_t rhs) const {
        Channel copy = *this;
        copy -= rhs;
        return copy;
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
