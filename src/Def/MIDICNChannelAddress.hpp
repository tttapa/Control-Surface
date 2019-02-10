/* ✔ */

#pragma once

#include <Def/Def.hpp>

/// A struct for saving a MIDI address consisting of a 7-bit address, a 4-bit
/// channel, and a 4-bit cable number.
struct __attribute__((packed)) RawMIDICNChannelAddress {
    bool valid : 1;
    uint8_t address : 7;
    uint8_t channel : 4;
    uint8_t cableNumber : 4;
};

/// A class for saving a MIDI channel and cable number.
class MIDICNChannel {
    friend class MIDICNChannelAddress;

  public:
    MIDICNChannel() : addresses{0, 0, 0, 0} {}
    MIDICNChannel(Channel channel, int cableNumber = 0)
        : addresses{1, 0, (uint8_t)channel.getRaw(), (uint8_t)cableNumber} {}

  private:
    RawMIDICNChannelAddress addresses;
};

/// A class for saving an offset to a MIDI address.
/// It can be added to a MIDICNChannelAddress.
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

/// A type-safe utility class for saving a MIDI address consisting of a 7-bit 
/// address, a 4-bit channel, and a 4-bit cable number.
class MIDICNChannelAddress {
  public:
    MIDICNChannelAddress() : addresses{0, 0, 0, 0} {}
    MIDICNChannelAddress(int address, Channel channel = CHANNEL_1,
                         int cableNumber = 0x0)
        : addresses{1, (uint8_t)address, (uint8_t)channel.getRaw(),
                    (uint8_t)cableNumber} {
    } // Deliberate overflow for negative numbers
    MIDICNChannelAddress(Channel channel, int cableNumber = 0x0)
        : addresses{1, 0, (uint8_t)channel.getRaw(), (uint8_t)cableNumber} {
    } // Deliberate overflow for negative numbers
    MIDICNChannelAddress(const MIDICNChannel &address)
        : addresses(address.addresses) {}

    MIDICNChannelAddress &operator+=(const RelativeMIDICNChannelAddress &rhs);

    MIDICNChannelAddress &operator-=(const RelativeMIDICNChannelAddress &rhs);

    MIDICNChannelAddress
    operator+(const RelativeMIDICNChannelAddress &rhs) const;

    MIDICNChannelAddress
    operator-(const RelativeMIDICNChannelAddress &rhs) const;

    bool operator==(const MIDICNChannelAddress &rhs) const;

    bool operator!=(const MIDICNChannelAddress &rhs) const;

    /// Get the address [0, 127].
    uint8_t getAddress() const { return addresses.address; }

    /// Get the channel [1, 16]
    Channel getChannel() const { return Channel{int8_t(addresses.channel)}; }

    /// Get the cable number [0, 15]
    uint8_t getCableNumber() const { return addresses.cableNumber; }

    /// Check if the MIDI address is valid.
    bool isValid() const { return addresses.valid; }

    /// Check if the MIDI address is valid.
    /// @see    isValid
    explicit operator bool() const { return isValid(); }

    /// Check if two addresses match.
    static bool matchSingle(const MIDICNChannelAddress &toMatch,
                            const MIDICNChannelAddress &base);

    /// Check if an address falls within a range of addresses, starting with
    /// address `base`, with a given length.
    static bool matchAddressInRange(const MIDICNChannelAddress &toMatch,
                                    const MIDICNChannelAddress &base,
                                    uint8_t length);

  private:
    RawMIDICNChannelAddress addresses;
};