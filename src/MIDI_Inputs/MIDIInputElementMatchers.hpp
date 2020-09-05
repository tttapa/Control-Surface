#pragma once

#include "BankableMIDIMatcherHelpers.hpp"
#include "MIDIInputElement.hpp"

BEGIN_CS_NAMESPACE

// -------------------------------------------------------------------------- //

/// Matcher for MIDI messages with 1 data byte, such as Channel Pressure
/// and Program Change.
struct OneByteMIDIMatcher {
    OneByteMIDIMatcher(MIDIChannelCN address) : address(address) {}

    struct Result {
        bool match;
        uint8_t value;
    };

    Result operator()(ChannelMessageMatcher m) {
        if (!MIDIChannelCN::matchSingle(m.getChannelCN(), address))
            return {false, 0};
        uint8_t value = m.data1;
        return {true, value};
    }

    MIDIChannelCN address;
};

// -------------------------------------------------------------------------- //

/// Matcher for MIDI messages with 2 data bytes, such as Note On/Off, Control
/// Change, Key Pressure. Matches a single address.
struct TwoByteMIDIMatcher {
    TwoByteMIDIMatcher(MIDIAddress address) : address(address) {}

    struct Result {
        bool match;
        uint8_t value;
    };

    Result operator()(ChannelMessageMatcher m) {
        if (!MIDIAddress::matchSingle(m.getAddress(), address))
            return {false, 0};
        uint8_t value = m.type == MIDIMessageType::NOTE_OFF ? 0 : m.data2;
        return {true, value};
    }

    MIDIAddress address;
};

// -------------------------------------------------------------------------- //

/// Matcher for MIDI messages with 2 data bytes, such as Note On/Off, Control
/// Change, Key Pressure. Matches ranges of addresses on a single channel and
/// cable.
struct TwoByteRangeMIDIMatcher {
    TwoByteRangeMIDIMatcher(MIDIAddress address, uint8_t length)
        : address(address), length(length) {}

    struct Result {
        bool match;
        uint8_t value;
        uint8_t index;
    };

    Result operator()(ChannelMessageMatcher m) {
        if (!MIDIAddress::matchAddressInRange(m.getAddress(), address, length))
            return {false, 0, 0};
        uint8_t value = m.type == MIDIMessageType::NOTE_OFF ? 0 : m.data2;
        uint8_t index = m.data1 - address.getAddress();
        return {true, value, index};
    }

    MIDIAddress address;
    uint8_t length;
};

// -------------------------------------------------------------------------- //

/// Matcher for MIDI messages with 2 data bytes, such as Note On/Off, Control
/// Change, Key Pressure. Matches a single address over multiple banks.
template <uint8_t BankSize>
struct BankableTwoByteMIDIMatcher {
    /// Constructor.
    BankableTwoByteMIDIMatcher(BankConfig<BankSize> config, MIDIAddress address)
        : config(config), address(address) {}

    struct Result {
        bool match;
        uint8_t value;
        uint8_t bankIndex;
    };

    Result operator()(ChannelMessageMatcher m) {
        using BankableMIDIMatcherHelpers::getBankIndex;
        using BankableMIDIMatcherHelpers::matchBankable;
        if (!matchBankable(m.getAddress(), address, config))
            return {false, 0, 0};
        uint8_t value = m.type == MIDIMessageType::NOTE_OFF ? 0 : m.data2;
        uint8_t bankIndex = getBankIndex(m.getAddress(), address, config);
        return {true, value, bankIndex};
    }

    Bank<BankSize> &getBank() { return config.bank; }
    const Bank<BankSize> &getBank() const { return config.bank; }
    BankType getBankType() const { return config.type; }
    static constexpr setting_t getBankSize() { return BankSize; }

    /// Get the current bank setting.
    /// @see    @ref Bank<N>::getSelection()
    setting_t getSelection() const { return getBank().getSelection(); }

    BaseBankConfig<BankSize> config;
    MIDIAddress address;
};

// -------------------------------------------------------------------------- //

/// Matcher for MIDI messages with 2 data bytes, such as Note On/Off, Control
/// Change, Key Pressure. Matches a range of addresses over multiple banks.
template <uint8_t BankSize>
struct BankableTwoByteRangeMIDIMatcher {
    /// Constructor.
    BankableTwoByteRangeMIDIMatcher(BankConfig<BankSize> config,
                                    MIDIAddress address, uint8_t length)
        : config(config), address(address), length(length) {}

    /**
     * @brief   Check if the given address is part of the bank relative to the
     *          base address.
     * 
     * @todo    This is very hard to explain without a specific example ...
     * 
     * @param   toMatch 
     *          The address to check.
     * @param   base
     *          The base address (the address of bank setting 0).
     * @param   length
     *          The length of the range.
     */
    bool matchBankableInRange(uint8_t toMatch, uint8_t base,
                              uint8_t length) const {
        uint8_t diff = toMatch - base;
        return toMatch >= base &&
               diff < BankSize * getBank().getTracksPerBank() &&
               diff % getBank().getTracksPerBank() < length;
    }

    /**
     * @brief   If matchBankableAddressInRange returned true, get the index of
     *          the message in the range.
     */
    uint8_t getRangeIndex(MIDIAddress target, MIDIAddress base) const {
        uint8_t diff = target.getAddress() - base.getAddress();
        if (getBankType() == CHANGE_ADDRESS)
            diff %= getBank().getTracksPerBank();
        return diff;
    }

    /**
     * @brief   Check whether a given address is within a range of given length
     *          starting from the given base address.
     * 
     * @param   toMatch
     *          The address to check
     * @param   base
     *          The base address, start of the range.
     * @param   length
     *          The length of the range.
     */
    static bool inRange(uint8_t toMatch, uint8_t base, uint8_t length) {
        return (base <= toMatch) && (toMatch - base < length);
    }

    /**
     * @brief   Check whether a given address is part of the bank relative to
     *          the base address and within a range with a given length.
     * 
     * @param   toMatch 
     *          The address to check.
     * @param   base
     *          The base address (the address of bank setting 0).
     * @param   length
     *          The length of the range.
     */
    bool matchBankableAddressInRange(MIDIAddress toMatch,
                                     MIDIAddress base) const {
        using BankableMIDIMatcherHelpers::matchBankable;
        if (!toMatch.isValid() || !base.isValid())
            return false;
        switch (getBankType()) {
            case CHANGE_ADDRESS: {
                return toMatch.getChannel() == base.getChannel() &&
                       toMatch.getCableNumber() == base.getCableNumber() &&
                       matchBankableInRange(toMatch.getAddress(),
                                            base.getAddress(), length);
            }
            case CHANGE_CHANNEL: {
                return inRange(toMatch.getAddress(), base.getAddress(),
                               length) &&
                       toMatch.getCableNumber() == base.getCableNumber() &&
                       matchBankable(toMatch.getRawChannel(),
                                     base.getRawChannel(), getBank());
            }
            case CHANGE_CABLENB: {
                return inRange(toMatch.getAddress(), base.getAddress(),
                               length) &&
                       toMatch.getChannel() == base.getChannel() &&
                       matchBankable(toMatch.getRawCableNumber(),
                                     base.getRawCableNumber(), getBank());
            }
            default: return false;
        }
    }

    struct Result {
        bool match;
        uint8_t value;
        uint8_t bankIndex;
        uint8_t index;
    };

    Result operator()(ChannelMessageMatcher m) {
        using BankableMIDIMatcherHelpers::getBankIndex;
        if (!matchBankableAddressInRange(m.getAddress(), address))
            return {false, 0, 0, 0};
        uint8_t value = m.type == MIDIMessageType::NOTE_OFF ? 0 : m.data2;
        uint8_t bankIndex = getBankIndex(m.getAddress(), address, config);
        uint8_t rangeIndex = getRangeIndex(m.getAddress(), address);
        return {true, value, bankIndex, rangeIndex};
    }

    Bank<BankSize> &getBank() { return config.bank; }
    const Bank<BankSize> &getBank() const { return config.bank; }
    BankType getBankType() const { return config.type; }
    static constexpr setting_t getBankSize() { return BankSize; }

    /// Get the current bank setting.
    /// @see    @ref Bank<N>::getSelection()
    setting_t getSelection() const { return getBank().getSelection(); }

    BaseBankConfig<BankSize> config;
    MIDIAddress address;
    uint8_t length;
};

// -------------------------------------------------------------------------- //

END_CS_NAMESPACE