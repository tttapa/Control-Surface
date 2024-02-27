#pragma once

#include "MIDIInputElementMatchers.hpp"

BEGIN_CS_NAMESPACE

// -------------------------------------------------------------------------- //

/// Generic base class for classes that listen for MIDI Note, Control Change and
/// Key Pressure events on a range of addresses and save their values.
///
/// @tparam Type
///         The type of MIDI messages to listen for:
///         - @ref MIDIMessageType::NoteOn
///         - @ref MIDIMessageType::ControlChange
///         - @ref MIDIMessageType::KeyPressure
/// @tparam RangeLen
///         The length of the range of addresses to listen to.
template <MIDIMessageType Type, uint8_t RangeLen>
class NoteCCKPRange
    : public MatchingMIDIInputElement<Type, TwoByteRangeMIDIMatcher> {
  public:
    using Matcher = TwoByteRangeMIDIMatcher;

    /// @param  address
    ///         The first address of the range to listen to.
    NoteCCKPRange(MIDIAddress address)
        : MatchingMIDIInputElement<Type, Matcher>({address, RangeLen}) {}

  private:
    bool handleUpdateImpl(typename Matcher::Result match) {
        bool newdirty = values[match.index] != match.value;
        values[match.index] = match.value;
        return newdirty;
    }

    void handleUpdate(typename Matcher::Result match) override {
        dirty |= handleUpdateImpl(match);
    }

  public:
    /// @name Data access
    /// @{

    /// Get the most recent MIDI value that was received for the given index
    /// in the range.
    uint8_t getValue(uint8_t index) const { return values[index]; }

    /// @}

    /// Reset all values to zero.
    void reset() override {
        values = {{}};
        dirty = true;
    }

    /// @name   Detecting changes
    /// @{

    /// Check if the value was updated since the last time the dirty flag was
    /// cleared.
    bool getDirty() const { return dirty; }
    /// Clear the dirty flag.
    void clearDirty() { dirty = false; }

    /// @}

  private:
    AH::Array<uint8_t, RangeLen> values = {{}};
    bool dirty = true;
};

/// Class that listen for MIDI Note events on a range of addresses and
/// saves their values.
/// @ingroup    MIDIInputElements
template <uint8_t RangeLen>
using NoteRange = NoteCCKPRange<MIDIMessageType::NoteOn, RangeLen>;

/// Class that listen for MIDI Control Change events on a range of addresses and
/// saves their values.
/// @ingroup    MIDIInputElements
template <uint8_t RangeLen>
using CCRange = NoteCCKPRange<MIDIMessageType::ControlChange, RangeLen>;

/// Class that listen for MIDI Key Pressure events on a range of addresses and
/// saves their values.
/// @ingroup    MIDIInputElements
template <uint8_t RangeLen>
using KPRange = NoteCCKPRange<MIDIMessageType::KeyPressure, RangeLen>;

// -------------------------------------------------------------------------- //

namespace Bankable {

/// Generic base class for classes that listen for MIDI Note, Control Change and
/// Key Pressure events on a range of addresses and save their values. This
/// version listens for the range over different banks.
///
/// @tparam Type
///         The type of MIDI messages to listen for:
///         - @ref MIDIMessageType::NoteOn
///         - @ref MIDIMessageType::ControlChange
///         - @ref MIDIMessageType::KeyPressure
/// @tparam BankSize
///         The number of banks.
/// @tparam RangeLen
///         The length of the range of addresses to listen to.
template <MIDIMessageType Type, uint8_t BankSize, uint8_t RangeLen>
class NoteCCKPRange : public BankableMatchingMIDIInputElement<
                          Type, BankableTwoByteRangeMIDIMatcher<BankSize>> {
  public:
    using Matcher = BankableTwoByteRangeMIDIMatcher<BankSize>;

    /// @param  config
    ///         The bank configuration to use.
    /// @param  address
    ///         The base address to listen to.
    NoteCCKPRange(BankConfig<BankSize> config, MIDIAddress address)
        : BankableMatchingMIDIInputElement<Type, Matcher>(
              {config, address, RangeLen}) {}

  protected:
    bool handleUpdateImpl(typename Matcher::Result match) {
        bool newdirty = values[match.bankIndex][match.index] != match.value &&
                        match.bankIndex == this->getActiveBank();
        // Only mark dirty if the value of the active bank changed
        values[match.bankIndex][match.index] = match.value;
        return newdirty;
    }

    void handleUpdate(typename Matcher::Result match) override {
        dirty |= handleUpdateImpl(match);
    }

  public:
    /// @name Data access
    /// @{

    /// Get the most recent MIDI value that was received for the active bank.
    uint8_t getValue(uint8_t index) const {
        return values[this->getActiveBank()][index];
    }
    /// Get the most recent MIDI value that was received for the given bank.
    uint8_t getValue(uint8_t bank, uint8_t index) const {
        return values[bank][index];
    }

    /// @}

    /// Reset all values to zero.
    void reset() override {
        values = {{{}}};
        dirty = true;
    }

    /// @name   Detecting changes
    /// @{

    /// Check if the value was updated since the last time the dirty flag was
    /// cleared.
    bool getDirty() const { return dirty; }
    /// Clear the dirty flag.
    void clearDirty() { dirty = false; }

    /// @}

  protected:
    void onBankSettingChange() override { dirty = true; }

  private:
    AH::Array2D<uint8_t, BankSize, RangeLen> values = {{{}}};

  protected:
    bool dirty = true;
};

/// Class that listens for MIDI Note events on a range of addresses and
/// saves their values. This version listens for the range over different banks.
/// @ingroup    BankableMIDIInputElements
template <uint8_t BankSize, uint8_t RangeLen>
using NoteRange = NoteCCKPRange<MIDIMessageType::NoteOn, BankSize, RangeLen>;

/// Class that listens for MIDI Control Change events on a range of addresses
/// and saves their values. This version listens for the range over different
/// banks.
/// @ingroup    BankableMIDIInputElements
template <uint8_t BankSize, uint8_t RangeLen>
using CCRange =
    NoteCCKPRange<MIDIMessageType::ControlChange, BankSize, RangeLen>;

/// Class that listens for MIDI Key Pressure events on a range of addresses and
/// saves their values. This version listens for the range over different banks.
/// @ingroup    BankableMIDIInputElements
template <uint8_t BankSize, uint8_t RangeLen>
using KPRange = NoteCCKPRange<MIDIMessageType::KeyPressure, BankSize, RangeLen>;

} // namespace Bankable

END_CS_NAMESPACE
