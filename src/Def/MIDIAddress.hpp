/* ✔ */

#pragma once

#include <Def/Def.hpp>

BEGIN_CS_NAMESPACE

/// A struct for saving a MIDI address consisting of a 7-bit address, a 4-bit
/// channel, and a 4-bit cable number.
struct __attribute__((packed)) RawMIDIAddress {
    bool valid : 1;
    uint8_t address : 7;
    uint8_t channel : 4;
    uint8_t cableNumber : 4;
};

/// A class for saving a MIDI channel and cable number.
class MIDIChannelCN {
    friend class MIDIAddress;

  public:
    constexpr MIDIChannelCN() : addresses{0, 0, 0, 0} {}
    constexpr MIDIChannelCN(Channel channel, int cableNumber = 0)
        : addresses{
              1,
              0,
              (uint8_t)channel.getRaw(),
              (uint8_t)cableNumber,
          } {}

    /// Get the channel [1, 16].
    constexpr Channel getChannel() const {
        return Channel{int8_t(addresses.channel)};
    }

    /// Get the channel as an integer [0, 15].
    constexpr uint8_t getRawChannel() const { return addresses.channel; }

    /// Get the cable number [0, 15].
    constexpr uint8_t getCableNumber() const { return addresses.cableNumber; }

    /// Check if the MIDI address is valid.
    constexpr bool isValid() const { return addresses.valid; }

    /// Check if the MIDI address is valid.
    /// @see    isValid
    constexpr explicit operator bool() const { return isValid(); }

    constexpr static MIDIChannelCN invalid() { return {}; }

  private:
    RawMIDIAddress addresses;
};

/// @deprecated
using MIDICNChannel [[deprecated("Use MIDIChannelCN instead")]] = MIDIChannelCN;

/// A class for saving an offset to a MIDI address.
/// It can be added to a MIDIAddress.
class RelativeMIDIAddress {
    friend class MIDIAddress;

  public:
    constexpr RelativeMIDIAddress() : addresses{0, 0, 0, 0} {}
    constexpr RelativeMIDIAddress(int deltaAddress, int deltaChannel = 0,
                                  int deltaCableNumber = 0)
        : addresses{
              1,
              (uint8_t)deltaAddress,
              (uint8_t)deltaChannel,
              (uint8_t)deltaCableNumber,
          } {}
    constexpr bool isValid() const { return addresses.valid; }

    /// Compound addition. Element-wise addition, result is valid if both 
    /// operands were valid.
    RelativeMIDIAddress &operator+=(RelativeMIDIAddress that);

  private:
    RawMIDIAddress addresses;
    static_assert(((-1) & 0x7F) == 0x7F,
                  "Negative numbers must be two's complement");
};

/// A type-safe utility class for saving a MIDI address consisting of a 7-bit
/// address, a 4-bit channel, and a 4-bit cable number.
class MIDIAddress {
  public:
    /// @name   Constructors
    /// @{

    /// Default constructor, creates an invalid address.
    constexpr MIDIAddress()
        : addresses{
              0,
              0,
              0,
              0,
          } {}

    /** 
     * @brief Constructor.
     * 
     * @param   address 
     *          The 7-bit MIDI address. Depending on the message type, this can
     *          be the MIDI note number, the number of the MIDI Control Change
     *          Controller, etc.  
     *          Must be a number in the range [0, 127].
     * @param   channelCN 
     *          The MIDI Channel and the MIDI USB cable number.
     */
    constexpr MIDIAddress(int address, MIDIChannelCN channelCN)
        : addresses{
              1,
              (uint8_t)address,
              channelCN.getRawChannel(),
              channelCN.getCableNumber(),
          } {}

    /** 
     * @brief Constructor.
     * 
     * @param   address 
     *          The 7-bit MIDI address.  
     *          Depending on the message type, this can be the MIDI note number,
     *          the number of the MIDI Control Change Controller, etc.  
     *          Must be a number in the range [0, 127].
     * @param   channel
     *          The MIDI Channel.  
     *          Use the constants @ref CHANNEL_1 through @ref CHANNEL_16.
     * @param   cableNumber 
     *          The MIDI USB cable number.  
     *          Must be a number in the range [0, 15].
     */
    constexpr MIDIAddress(int address, Channel channel = CHANNEL_1,
                          int cableNumber = 0x0)
        : addresses{
              1,
              (uint8_t)address,
              (uint8_t)channel.getRaw(),
              (uint8_t)cableNumber,
          } {} // Deliberate overflow for negative numbers

    /** 
     * @brief Constructor.
     * 
     * @param   channel
     *          The MIDI Channel.  
     *          Use the constants @ref CHANNEL_1 through @ref CHANNEL_16.
     * @param   cableNumber 
     *          The MIDI USB cable number.  
     *          Must be a number in the range [0, 15].
     */
    constexpr MIDIAddress(Channel channel, int cableNumber = 0x0)
        : addresses{
              1,
              0,
              (uint8_t)channel.getRaw(),
              (uint8_t)cableNumber,
          } {} // Deliberate overflow for negative numbers

    /** 
     * @brief Constructor.
     * 
     * @param   address 
     *          The MIDI Channel and the MIDI USB cable number.  
     */
    constexpr MIDIAddress(const MIDIChannelCN &address)
        : addresses(address.addresses) {}

    /// Return an invalid address.
    constexpr static MIDIAddress invalid() { return {}; }

    /// @}

  public:
    /// @name   Adding/subtracting offsets
    /// @{

    /// Add a relative offset to this address.
    MIDIAddress &operator+=(const RelativeMIDIAddress &rhs);

    /// Subtract a relative offset from this address.
    MIDIAddress &operator-=(const RelativeMIDIAddress &rhs);

    /// Add a relative offset.
    MIDIAddress operator+(const RelativeMIDIAddress &rhs) const;

    /// Subtract a relative offset.
    MIDIAddress operator-(const RelativeMIDIAddress &rhs) const;

    /// @}

  public:
    /// @name   Member access
    /// @{

    /// Get the address [0, 127].
    constexpr uint8_t getAddress() const { return addresses.address; }

    /// Get the channel [CHANNEL_1, CHANNEL_16]
    constexpr Channel getChannel() const {
        return Channel{int8_t(addresses.channel)};
    }
    /// Get the channel as an integer [0, 15]
    constexpr uint8_t getRawChannel() const { return addresses.channel; }

    /// Get the cable number [0, 15]
    constexpr uint8_t getCableNumber() const { return addresses.cableNumber; }

    /// @}

  public:
    /// @name   Checks
    /// @{

    /// Check for equality: two addresses are equal if and only if they are both
    /// valid addresses and the MIDI address, MIDI channel and MIDI USB cable
    /// number are equal.
    /// @note   Invalid addresses are not equal nor inequal.
    constexpr bool operator==(const MIDIAddress &rhs) const {
        return this->addresses.valid && rhs.addresses.valid &&
               this->addresses.address == rhs.addresses.address &&
               this->addresses.channel == rhs.addresses.channel &&
               this->addresses.cableNumber == rhs.addresses.cableNumber;
    }

    /// Check for inequality: two addresses are not equal if and only if they
    /// are both valid addresses and have a MIDI address, MIDI channel or MIDI
    /// USB cable number that differs.
    /// @note   Invalid addresses are not equal nor inequal.
    constexpr bool operator!=(const MIDIAddress &rhs) const {
        return this->addresses.valid && rhs.addresses.valid &&
               !(this->addresses.address == rhs.addresses.address &&
                 this->addresses.channel == rhs.addresses.channel &&
                 this->addresses.cableNumber == rhs.addresses.cableNumber);
    }

    /// Check if the MIDI address is valid.
    constexpr bool isValid() const { return addresses.valid; }

    /// Check if the MIDI address is valid.
    /// @see    isValid
    constexpr explicit operator bool() const { return isValid(); }

    /// @}

  public:
    /// @name   Base functions for address pattern matching
    /// @{

    /// Check if two addresses match (are equal).
    static bool matchSingle(const MIDIAddress &toMatch,
                            const MIDIAddress &base);

    /// Check if an address falls within a range of MIDI addresses, starting
    /// with address `base`, with a given length.
    /// @retval true
    ///         `toMatch` and `base` are both valid addresses,
    ///         the MIDI address is within the given range, and the MIDI Channel
    ///         and MIDI USB Cable Number of `toMatch` and `base` are the same.
    /// @retval false
    ///         Otherwise.
    static bool matchAddressInRange(const MIDIAddress &toMatch,
                                    const MIDIAddress &base, uint8_t length);

    /// @}

  private:
    RawMIDIAddress addresses;
};

/// @deprecated
using MIDICNChannelAddress [[deprecated("Use MIDIAddress instead")]] =
    MIDIAddress;

END_CS_NAMESPACE