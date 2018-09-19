#pragma once

#include <stdint.h> // uint8_t

constexpr uint8_t CHANNEL_1 = 0;
constexpr uint8_t CHANNEL_2 = 1;
constexpr uint8_t CHANNEL_3 = 2;
constexpr uint8_t CHANNEL_4 = 3;
constexpr uint8_t CHANNEL_5 = 4;
constexpr uint8_t CHANNEL_6 = 5;
constexpr uint8_t CHANNEL_7 = 6;
constexpr uint8_t CHANNEL_8 = 7;
constexpr uint8_t CHANNEL_9 = 8;
constexpr uint8_t CHANNEL_10 = 9;
constexpr uint8_t CHANNEL_11 = 10;
constexpr uint8_t CHANNEL_12 = 11;
constexpr uint8_t CHANNEL_13 = 12;
constexpr uint8_t CHANNEL_14 = 13;
constexpr uint8_t CHANNEL_15 = 14;
constexpr uint8_t CHANNEL_16 = 15;

class MIDICNChannelAddress {
  public:
    MIDICNChannelAddress() : addresses{0, 0, 0, 0} {}
    MIDICNChannelAddress(int8_t address, int8_t channel = CHANNEL_1,
                         int8_t cableNumber = 0)
        : addresses{1, (uint8_t)address, (uint8_t)channel,
                    (uint8_t)cableNumber} {
    } // Deliberate overflow for negative numbers
    /* MIDICNChannelAddress(uint8_t address, uint8_t channel = CHANNEL_1,
                         uint8_t cableNumber = 0)
        : addresses{1, address, channel, cableNumber} {} */

    MIDICNChannelAddress &operator+=(const MIDICNChannelAddress &rhs) {
        if (!this->isValid() || !rhs.isValid()) {
            this->addresses.valid = false;
        } else {
            this->addresses.address += rhs.addresses.address;
            this->addresses.channel += rhs.addresses.channel;
            this->addresses.cableNumber += rhs.addresses.cableNumber;
        }
        return *this;
    }

    MIDICNChannelAddress operator+(const MIDICNChannelAddress &rhs) const {
        MIDICNChannelAddress copy = *this;
        copy += rhs;
        return copy;
    }

    bool operator==(const MIDICNChannelAddress &rhs) const {
        return this->addresses.valid == rhs.addresses.valid &&
               this->addresses.address == rhs.addresses.address &&
               this->addresses.channel == rhs.addresses.channel &&
               this->addresses.cableNumber == rhs.addresses.cableNumber;
    }

    bool operator!=(const MIDICNChannelAddress &rhs) const {
        return !(*this == rhs);
    }

    uint8_t getAddress() const { return addresses.address; }

    uint8_t getChannel() const { return addresses.channel; }

    uint8_t getCableNumber() const { return addresses.cableNumber; }

    bool isValid() const { return addresses.valid; }

  private:
    struct __attribute__((packed)) {
        bool valid : 1;
        uint8_t address : 7;
        uint8_t channel : 4;
        uint8_t cableNumber : 4;
    } addresses;
};