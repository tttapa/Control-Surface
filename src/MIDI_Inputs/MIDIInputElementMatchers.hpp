#pragma once

#include "BankableMIDIMatcherHelpers.hpp"
#include "MIDIInputElement.hpp"

BEGIN_CS_NAMESPACE

/// @addtogroup MIDIInputMatchers
/// @{

// -------------------------------------------------------------------------- //

/// Matcher for MIDI messages with 1 data byte, such as Channel Pressure
/// and Program Change.
struct OneByteMIDIMatcher {
    OneByteMIDIMatcher(MIDIChannelCable address) : address(address) {}

    struct Result {
        bool match;
        uint8_t value;
    };

    Result operator()(ChannelMessage m) {
        if (!MIDIChannelCable::matchSingle(m.getChannelCable(), address))
            return {false, 0};
        uint8_t value = m.data1;
        return {true, value};
    }

    MIDIChannelCable address;
};

// -------------------------------------------------------------------------- //

/// Matcher for MIDI messages with 2 data bytes, such as Note On/Off, Control
/// Change, Key Pressure (but not Pitch Bend). Matches a single address.
struct TwoByteMIDIMatcher {
    TwoByteMIDIMatcher(MIDIAddress address) : address(address) {}

    struct Result {
        bool match;
        uint8_t value;
    };

    Result operator()(ChannelMessage m) {
        if (!MIDIAddress::matchSingle(m.getAddress(), address))
            return {false, 0};
        uint8_t value =
            m.getMessageType() == MIDIMessageType::NoteOff ? 0 : m.getData2();
        return {true, value};
    }

    MIDIAddress address;
};

// -------------------------------------------------------------------------- //

/// Matcher for MIDI Pitch Bend messages. Matches a single address.
struct PitchBendMIDIMatcher {
    PitchBendMIDIMatcher(MIDIChannelCable address) : address(address) {}

    struct Result {
        bool match;
        uint16_t value;
    };

    Result operator()(ChannelMessage m) {
        if (!MIDIChannelCable::matchSingle(m.getChannelCable(), address))
            return {false, 0};
        uint16_t value = (m.data2 << 7) | m.data1;
        return {true, value};
    }

    MIDIChannelCable address;
};

// -------------------------------------------------------------------------- //

/// Matcher for MIDI messages with 2 data bytes, such as Note On/Off, Control
/// Change, Key Pressure (but not Pitch Bend). Matches ranges of addresses on a
/// single channel and cable.
struct TwoByteRangeMIDIMatcher {
    TwoByteRangeMIDIMatcher(MIDIAddress address, uint8_t length)
        : address(address), length(length) {}

    struct Result {
        bool match;
        uint8_t value;
        uint8_t index;
    };

    Result operator()(ChannelMessage m) {
        if (!MIDIAddress::matchAddressInRange(m.getAddress(), address, length))
            return {false, 0, 0};
        uint8_t value =
            m.getMessageType() == MIDIMessageType::NoteOff ? 0 : m.getData2();
        uint8_t index = m.getData1() - address.getAddress();
        return {true, value, index};
    }

    MIDIAddress address;
    uint8_t length;
};

// -------------------------------------------------------------------------- //

/// Matcher for MIDI messages with 1 data byte, such as Channel Pressure
/// and Program Change. Matches a single address over multiple banks.
template <uint8_t BankSize>
struct BankableOneByteMIDIMatcher {
    BankableOneByteMIDIMatcher(
        BankConfig<BankSize, BankType::ChangeChannel> config,
        MIDIChannelCable address)
        : config(config), address(address) {}

    struct Result {
        bool match;
        uint8_t value;
        uint8_t bankIndex;
    };

    Result operator()(ChannelMessage m) {
        using BankableMIDIMatcherHelpers::getBankIndex;
        using BankableMIDIMatcherHelpers::matchBankable;
        if (!matchBankable(m.getChannelCable(), address, config))
            return {false, 0, 0};
        uint8_t value = m.data1;
        uint8_t bankIndex = getBankIndex(m.getChannelCable(), address, config);
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
    MIDIChannelCable address;
};

// -------------------------------------------------------------------------- //

/// Matcher for MIDI messages with 2 data bytes, such as Note On/Off, Control
/// Change, Key Pressure. Matches a single address over multiple banks.
template <uint8_t BankSize>
struct BankableTwoByteMIDIMatcher {
    BankableTwoByteMIDIMatcher(BankConfig<BankSize> config, MIDIAddress address)
        : config(config), address(address) {}

    struct Result {
        bool match;
        uint8_t value;
        uint8_t bankIndex;
    };

    Result operator()(ChannelMessage m) {
        using BankableMIDIMatcherHelpers::getBankIndex;
        using BankableMIDIMatcherHelpers::matchBankable;
        if (!matchBankable(m.getAddress(), address, config))
            return {false, 0, 0};
        uint8_t value =
            m.getMessageType() == MIDIMessageType::NoteOff ? 0 : m.getData2();
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

/// Matcher for MIDI Pitch Bend messages. Matches a single address over multiple
/// banks.
template <uint8_t BankSize>
struct BankablePitchBendMIDIMatcher {
    BankablePitchBendMIDIMatcher(
        BankConfig<BankSize, BankType::ChangeChannel> config,
        MIDIChannelCable address)
        : config(config), address(address) {}

    struct Result {
        bool match;
        uint16_t value;
        uint8_t bankIndex;
    };

    Result operator()(ChannelMessage m) {
        using BankableMIDIMatcherHelpers::getBankIndex;
        using BankableMIDIMatcherHelpers::matchBankable;
        if (!matchBankable(m.getChannelCable(), address, config))
            return {false, 0, 0};
        uint16_t value = (m.data2 << 7) | m.data1;
        uint8_t bankIndex = getBankIndex(m.getChannelCable(), address, config);
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
    MIDIChannelCable address;
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

    struct Result {
        bool match;
        uint8_t value;
        uint8_t bankIndex;
        uint8_t index;
    };

    Result operator()(ChannelMessage m) {
        using BankableMIDIMatcherHelpers::getBankIndex;
        using BankableMIDIMatcherHelpers::getRangeIndex;
        using BankableMIDIMatcherHelpers::matchBankableInRange;
        if (!matchBankableInRange(m.getAddress(), address, config, length))
            return {false, 0, 0, 0};
        uint8_t value = m.getMessageType() == m.NoteOff ? 0 : m.getData2();
        uint8_t bankIndex = getBankIndex(m.getAddress(), address, config);
        uint8_t rangeIndex = getRangeIndex(m.getAddress(), address, config);
        return {true, value, bankIndex, rangeIndex};
    }

    Bank<BankSize> &getBank() { return config.bank; }
    const Bank<BankSize> &getBank() const { return config.bank; }
    static constexpr setting_t getBankSize() { return BankSize; }

    /// Get the current bank setting.
    /// @see    @ref Bank<N>::getSelection()
    setting_t getSelection() const { return config.bank.getSelection(); }

    BaseBankConfig<BankSize> config;
    MIDIAddress address;
    uint8_t length;
};

// -------------------------------------------------------------------------- //

/// @}

END_CS_NAMESPACE