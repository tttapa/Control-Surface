/* ✔ */

#pragma once

#include <stdint.h> // int8_t
#include <Settings/NamespaceSettings.hpp>

BEGIN_CS_NAMESPACE

/** 
 * A type-safe class for MIDI channels.
 */
class Channel {
  public:
    /**
     * @brief   Create a MIDI Channel object.
     * 
     * @param   zeroBasedChannel
     *          The zero-based channel (0 is the first channel).
     */
    explicit constexpr Channel(int8_t zeroBasedChannel)
        : zeroBasedChannel(zeroBasedChannel) {}

    /**
     * @brief   Get the channel as an integer.
     * 
     * @return  The zero-based channel (0 is the first channel).
     */
    constexpr int8_t getRaw() const { return zeroBasedChannel; }

    /**
     * @brief   Create a channel.
     * 
     * @param   oneBasedChannel
     *          The channel number (1 is the first channel).
     */
    static constexpr Channel createChannel(int8_t oneBasedChannel) {
        return Channel{int8_t(oneBasedChannel - 1)};
    }

    /**
     * @brief   Check if two channels are the same.
     * 
     * @param   rhs
     *          The other channel to compare this channel to.
     */
    constexpr bool operator==(const Channel &rhs) const {
        return this->zeroBasedChannel == rhs.zeroBasedChannel;
    }

    /**
     * @brief   Add an offset.
     * 
     * @param   rhs
     *          The offset to add to this channel.
     */
    Channel &operator+=(const int8_t rhs) {
        this->zeroBasedChannel += rhs;
        return *this;
    }

    /**
     * @brief   Add an offset to a channel.
     * 
     * @param   rhs
     *          The offset to add to the channel.
     */
    Channel operator+(const int8_t rhs) const {
        Channel copy = *this;
        copy += rhs;
        return copy;
    }

    /**
     * @brief   Subtract an offset.
     * 
     * @param   rhs
     *          The offset to subtract from this channel.
     */
    Channel &operator-=(const int8_t rhs) {
        this->zeroBasedChannel -= rhs;
        return *this;
    }

    /**
     * @brief   Subtract an offset from a channel.
     * 
     * @param   rhs
     *          The offset to subtract from the channel.
     */
    Channel operator-(const int8_t rhs) const {
        Channel copy = *this;
        copy -= rhs;
        return copy;
    }

  private:
    int8_t zeroBasedChannel;
};

/** 
 * @brief   A literal operator to create MIDI Channels.
 * 
 * @param   ch
 *          The (one-based) MIDI channel (1 is the first channel).
 */
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

END_CS_NAMESPACE