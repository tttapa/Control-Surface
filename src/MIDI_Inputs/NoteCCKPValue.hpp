#pragma once

#include "MIDIInputElementMatchers.hpp"

BEGIN_CS_NAMESPACE

// -------------------------------------------------------------------------- //

/// Generic base class for classes that listen for MIDI Note, Control Change and
/// Key Pressure events on a single address and save their value.
///
/// @tparam Type
///         The type of MIDI messages to listen for:
///         - MIDIMessageType::NOTE_ON
///         - MIDIMessageType::CONTROL_CHANGE
///         - MIDIMessageType::KEY_PRESSURE
template <MIDIMessageType Type>
class NoteCCKPValue
    : public MatchingMIDIInputElement<Type, TwoByteMIDIMatcher> {
  public:
    /// Constructor.
    ///
    /// @param  address
    ///         The address to listen to.
    NoteCCKPValue(MIDIAddress address)
        : MatchingMIDIInputElement<Type, TwoByteMIDIMatcher>(address) {}

  private:
    void handleUpdate(typename TwoByteMIDIMatcher::Result match) override {
        dirty |= value != match.value;
        value = match.value;
    }

  public:
    /// Get the most recent MIDI value that was received.
    uint8_t getValue() const { return value; }
    /// Reset all values to zero.
    void reset() override {
        value = 0;
        dirty = true;
    }
    /// Check if the value was updated since the last time the dirty flag was
    /// cleared.
    bool getDirty() const { return dirty; }
    /// Clear the dirty flag.
    void clearDirty() { dirty = false; }

  private:
    uint8_t value = 0;
    bool dirty = true;
};

/// Class that listens for MIDI Note events on a single address and saves their
/// value.
/// @ingroup    MIDIInputElements
using NewNoteValue = NoteCCKPValue<MIDIMessageType::NOTE_ON>;

/// Class that listens for MIDI Control Change events on a single address and
/// saves their value.
/// @ingroup    MIDIInputElements
using NewCCValue = NoteCCKPValue<MIDIMessageType::CONTROL_CHANGE>;

/// Class that listens for MIDI Key Pressure events on a single address and
/// saves their value.
/// @ingroup    MIDIInputElements
using NewKPValue = NoteCCKPValue<MIDIMessageType::KEY_PRESSURE>;

// -------------------------------------------------------------------------- //

namespace Bankable {

/// Generic base class for classes that listen for MIDI Note, Control Change and
/// Key Pressure events on a single address and save their value. This version
/// listens accross multiple banks.
///
/// @tparam Type
///         The type of MIDI messages to listen for:
///         - MIDIMessageType::NOTE_ON
///         - MIDIMessageType::CONTROL_CHANGE
///         - MIDIMessageType::KEY_PRESSURE
/// @param  BankSize
///         The number of banks.
template <MIDIMessageType Type, uint8_t BankSize>
class NoteCCKPValue : public BankableMatchingMIDIInputElement<
                          Type, BankableTwoByteMIDIMatcher<BankSize>> {
  public:
    using Matcher = BankableTwoByteMIDIMatcher<BankSize>;

    /// Constructor.
    ///
    /// @param  config
    ///         The bank configuration to use.
    /// @param  address
    ///         The base address to listen to.
    NoteCCKPValue(BankConfig<BankSize> config, MIDIAddress address)
        : BankableMatchingMIDIInputElement<Type, Matcher>({config, address}) {}

  protected:
    void handleUpdate(typename Matcher::Result match) override {
        dirty |= values[match.bankIndex] != match.value;
        values[match.bankIndex] = match.value;
    }

    uint8_t getActiveBank() const { return this->matcher.getSelection(); }

  public:
    /// Get the most recent MIDI value that was received for the active bank.
    uint8_t getValue() const { return values[getActiveBank()]; }
    /// Get the most recent MIDI value that was received for the given bank.
    uint8_t getValue(uint8_t bank) const { return values[bank]; }
    /// Reset all values to zero.
    void reset() override {
        values = {{}};
        dirty = true;
    }
    /// Check if the value was updated since the last time the dirty flag was
    /// cleared.
    bool getDirty() const { return dirty; }
    /// Clear the dirty flag.
    void clearDirty() { dirty = false; }

  private:
    AH::Array<uint8_t, BankSize> values = {{}};
    bool dirty = true;
};

/// Class that listens for MIDI Note events on a single address and
/// saves their value. This version listens accross multiple banks.
/// @ingroup    BankableMIDIInputElements
template <uint8_t BankSize>
using NewNoteValue = NoteCCKPValue<MIDIMessageType::NOTE_ON, BankSize>;

/// Class that listens for MIDI Control Change events on a single address and
/// saves their value. This version listens accross multiple banks.
/// @ingroup    BankableMIDIInputElements
template <uint8_t BankSize>
using NewCCValue = NoteCCKPValue<MIDIMessageType::CONTROL_CHANGE, BankSize>;

/// Class that listens for MIDI Key Pressure events on a single address and
/// saves their value. This version listens accross multiple banks.
/// @ingroup    BankableMIDIInputElements
template <uint8_t BankSize>
using NewKPValue = NoteCCKPValue<MIDIMessageType::KEY_PRESSURE, BankSize>;

} // namespace Bankable

END_CS_NAMESPACE