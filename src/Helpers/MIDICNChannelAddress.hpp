#pragma once

#include <Def/Def.hpp>

struct __attribute__((packed)) RawMIDICNChannelAddress {
    bool valid : 1;
    uint8_t address : 7;
    uint8_t channel : 4;
    uint8_t cableNumber : 4;
};

class RelativeMIDICNChannelAddress {
    friend class MIDICNChannelAddress;

  public:
    RelativeMIDICNChannelAddress() : addresses{0, 0, 0, 0} {}
    RelativeMIDICNChannelAddress(int deltaAddress, int deltaChannel = 0,
                                 int deltaCableNumber = 0)
        : addresses{1, (uint8_t)deltaAddress, (uint8_t)deltaChannel,
                    (uint8_t)deltaCableNumber} {}
    bool isValid() const { return addresses.valid; }

  private:
    RawMIDICNChannelAddress addresses;
};

class MIDICNChannelAddress {
  public:
    MIDICNChannelAddress() : addresses{0, 0, 0, 0} {}
    MIDICNChannelAddress(int address, Channel channel = CHANNEL_1,
                         int cableNumber = 0)
        : addresses{1, (uint8_t)address, (uint8_t)channel,
                    (uint8_t)cableNumber} {
    } // Deliberate overflow for negative numbers
    MIDICNChannelAddress(Channel channel, int8_t cableNumber = 0)
        : addresses{1, 0, (uint8_t)channel, (uint8_t)cableNumber} {
    } // Deliberate overflow for negative numbers
    /* MIDICNChannelAddress(uint8_t address, uint8_t channel = CHANNEL_1,
                         uint8_t cableNumber = 0)
        : addresses{1, address, channel, cableNumber} {} */

    MIDICNChannelAddress &operator+=(const RelativeMIDICNChannelAddress &rhs) {
        if (!this->isValid() || !rhs.isValid()) {
            this->addresses.valid = false;
        } else {
            this->addresses.address += rhs.addresses.address;
            this->addresses.channel += rhs.addresses.channel;
            this->addresses.cableNumber += rhs.addresses.cableNumber;
        }
        return *this;
    }

    MIDICNChannelAddress
    operator+(const RelativeMIDICNChannelAddress &rhs) const {
        MIDICNChannelAddress copy = *this;
        copy += rhs;
        return copy;
    }

    bool operator==(const MIDICNChannelAddress &rhs) const {
        return this->addresses.valid && rhs.addresses.valid &&
               this->addresses.address == rhs.addresses.address &&
               this->addresses.channel == rhs.addresses.channel &&
               this->addresses.cableNumber == rhs.addresses.cableNumber;
    }

    bool operator!=(const MIDICNChannelAddress &rhs) const {
        return this->addresses.valid && rhs.addresses.valid &&
               !(this->addresses.address == rhs.addresses.address &&
                 this->addresses.channel == rhs.addresses.channel &&
                 this->addresses.cableNumber == rhs.addresses.cableNumber);
    }

    uint8_t getAddress() const { return addresses.address; }

    Channel getChannel() const { return Channel{int8_t(addresses.channel)}; }

    uint8_t getCableNumber() const { return addresses.cableNumber; }

    bool isValid() const { return addresses.valid; }

    explicit operator bool() const { return isValid(); }

    static bool matchSingle(const MIDICNChannelAddress &toMatch,
                            const MIDICNChannelAddress &base) {
        return base == toMatch;
    }

    static bool matchAddressInRange(const MIDICNChannelAddress &toMatch,
                                    const MIDICNChannelAddress &base,
                                    uint8_t length) {
        bool valid = base.addresses.valid && toMatch.addresses.valid;
        bool addressInRange =
            base.addresses.address <= toMatch.addresses.address &&
            base.addresses.address + length > toMatch.addresses.address;
        bool equalChannelAndCN =
            base.addresses.channel == toMatch.addresses.channel &&
            base.addresses.cableNumber == toMatch.addresses.cableNumber;
        return valid && addressInRange && equalChannelAndCN;
    }

  private:
    RawMIDICNChannelAddress addresses;
};