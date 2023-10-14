/* ✔ */

#pragma once

#include <AH/Arduino-Wrapper.h> // Print
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
    explicit constexpr Channel(uint8_t zeroBasedChannel)
        : zeroBasedChannel(zeroBasedChannel) {}

    /**
     * @brief   Get the channel as an integer.
     * 
     * @return  The zero-based channel (0 is the first channel).
     */
    constexpr uint8_t getRaw() const { return zeroBasedChannel; }

    /**
     * @brief   Get the channel as an integer.
     * 
     * @return  The one-based channel (1 is the first channel).
     */
    constexpr uint8_t getOneBased() const { return zeroBasedChannel + 1; }

    /**
     * @brief   Create a channel.
     * 
     * @param   oneBasedChannel
     *          The channel number (1 is the first channel).
     */
    static constexpr Channel createChannel(uint8_t oneBasedChannel) {
        return Channel {uint8_t(oneBasedChannel - 1)};
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
     * @brief   Check if two channels are the different.
     * 
     * @param   rhs
     *          The other channel to compare this channel to.
     */
    constexpr bool operator!=(const Channel &rhs) const {
        return this->zeroBasedChannel != rhs.zeroBasedChannel;
    }

    /**
     * @brief   Add an offset.
     * 
     * @param   rhs
     *          The offset to add to this channel.
     */
    Channel &operator+=(uint8_t rhs) {
        this->zeroBasedChannel += rhs;
        return *this;
    }

    /**
     * @brief   Add an offset to a channel.
     * 
     * @param   rhs
     *          The offset to add to the channel.
     */
    Channel operator+(uint8_t rhs) const {
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
    Channel &operator-=(uint8_t rhs) {
        this->zeroBasedChannel -= rhs;
        return *this;
    }

    /**
     * @brief   Subtract an offset from a channel.
     * 
     * @param   rhs
     *          The offset to subtract from the channel.
     */
    Channel operator-(uint8_t rhs) const {
        Channel copy = *this;
        copy -= rhs;
        return copy;
    }

  private:
    uint8_t zeroBasedChannel : 4;
};

constexpr Channel Channel_1 = Channel::createChannel(1);
constexpr Channel Channel_2 = Channel::createChannel(2);
constexpr Channel Channel_3 = Channel::createChannel(3);
constexpr Channel Channel_4 = Channel::createChannel(4);
constexpr Channel Channel_5 = Channel::createChannel(5);
constexpr Channel Channel_6 = Channel::createChannel(6);
constexpr Channel Channel_7 = Channel::createChannel(7);
constexpr Channel Channel_8 = Channel::createChannel(8);
constexpr Channel Channel_9 = Channel::createChannel(9);
constexpr Channel Channel_10 = Channel::createChannel(10);
constexpr Channel Channel_11 = Channel::createChannel(11);
constexpr Channel Channel_12 = Channel::createChannel(12);
constexpr Channel Channel_13 = Channel::createChannel(13);
constexpr Channel Channel_14 = Channel::createChannel(14);
constexpr Channel Channel_15 = Channel::createChannel(15);
constexpr Channel Channel_16 = Channel::createChannel(16);

#ifndef CHANNEL_1 // ArduinoCore-renesas defines this
constexpr Channel CHANNEL_1 CS_DEPREC("Use Channel_1 instead") = Channel_1;
constexpr Channel CHANNEL_2 CS_DEPREC("Use Channel_2 instead") = Channel_2;
constexpr Channel CHANNEL_3 CS_DEPREC("Use Channel_3 instead") = Channel_3;
constexpr Channel CHANNEL_4 CS_DEPREC("Use Channel_4 instead") = Channel_4;
constexpr Channel CHANNEL_5 CS_DEPREC("Use Channel_5 instead") = Channel_5;
constexpr Channel CHANNEL_6 CS_DEPREC("Use Channel_6 instead") = Channel_6;
constexpr Channel CHANNEL_7 CS_DEPREC("Use Channel_7 instead") = Channel_7;
constexpr Channel CHANNEL_8 CS_DEPREC("Use Channel_8 instead") = Channel_8;
constexpr Channel CHANNEL_9 CS_DEPREC("Use Channel_9 instead") = Channel_9;
constexpr Channel CHANNEL_10 CS_DEPREC("Use Channel_10 instead") = Channel_10;
constexpr Channel CHANNEL_11 CS_DEPREC("Use Channel_11 instead") = Channel_11;
constexpr Channel CHANNEL_12 CS_DEPREC("Use Channel_12 instead") = Channel_12;
constexpr Channel CHANNEL_13 CS_DEPREC("Use Channel_13 instead") = Channel_13;
constexpr Channel CHANNEL_14 CS_DEPREC("Use Channel_14 instead") = Channel_14;
constexpr Channel CHANNEL_15 CS_DEPREC("Use Channel_15 instead") = Channel_15;
constexpr Channel CHANNEL_16 CS_DEPREC("Use Channel_16 instead") = Channel_16;
#endif

Print &operator<<(Print &, Channel);

END_CS_NAMESPACE