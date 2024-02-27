#pragma once

#include "InterfaceMIDIInputElements.hpp"
#include "MIDIInputElementMatchers.hpp"

BEGIN_CS_NAMESPACE

// -------------------------------------------------------------------------- //

/// Generic base class for classes that listen for MIDI Note, Control Change and
/// Key Pressure events on a single address and save their value.
///
/// @tparam Type
///         The type of MIDI messages to listen for:
///         - @ref MIDIMessageType::NoteOn
///         - @ref MIDIMessageType::ControlChange
///         - @ref MIDIMessageType::KeyPressure
template <MIDIMessageType Type>
class NoteCCKPValue : public MatchingMIDIInputElement<Type, TwoByteMIDIMatcher>,
                      public Interfaces::IValue {
  public:
    using Matcher = TwoByteMIDIMatcher;

    /// @param  address
    ///         The address to listen to.
    NoteCCKPValue(MIDIAddress address)
        : MatchingMIDIInputElement<Type, TwoByteMIDIMatcher>(address) {}

  protected:
    bool handleUpdateImpl(typename Matcher::Result match) {
        bool newdirty = value != match.value;
        value = match.value;
        return newdirty;
    }

    void handleUpdate(typename Matcher::Result match) override {
        dirty |= handleUpdateImpl(match);
    }

  public:
    /// @name Data access
    /// @{

    /// Get the most recent MIDI value that was received.
    uint8_t getValue() const override { return value; }

    /// @}

    /// Reset all values to zero.
    void reset() override {
        value = 0;
        dirty = true;
    }

  private:
    uint8_t value = 0;
};

/// Class that listens for MIDI Note events on a single address and saves their
/// value.
/// @ingroup    MIDIInputElements
using NoteValue = NoteCCKPValue<MIDIMessageType::NoteOn>;

/// Class that listens for MIDI Control Change events on a single address and
/// saves their value.
/// @ingroup    MIDIInputElements
using CCValue = NoteCCKPValue<MIDIMessageType::ControlChange>;

/// Class that listens for MIDI Key Pressure events on a single address and
/// saves their value.
/// @ingroup    MIDIInputElements
using KPValue = NoteCCKPValue<MIDIMessageType::KeyPressure>;

// -------------------------------------------------------------------------- //

namespace Bankable {

/// Generic base class for classes that listen for MIDI Note, Control Change and
/// Key Pressure events on a single address and save their value. This version
/// listens accross multiple banks.
///
/// @tparam Type
///         The type of MIDI messages to listen for:
///         - @ref MIDIMessageType::NoteOn
///         - @ref MIDIMessageType::ControlChange
///         - @ref MIDIMessageType::KeyPressure
/// @tparam BankSize
///         The number of banks.
template <MIDIMessageType Type, uint8_t BankSize>
class NoteCCKPValue : public BankableMatchingMIDIInputElement<
                          Type, BankableTwoByteMIDIMatcher<BankSize>>,
                      public Interfaces::IValue {
  public:
    using Matcher = BankableTwoByteMIDIMatcher<BankSize>;

    /// @param  config
    ///         The bank configuration to use.
    /// @param  address
    ///         The base address to listen to.
    NoteCCKPValue(BankConfig<BankSize> config, MIDIAddress address)
        : BankableMatchingMIDIInputElement<Type, Matcher>({config, address}) {}

  protected:
    bool handleUpdateImpl(typename Matcher::Result match) {
        bool newdirty = values[match.bankIndex] != match.value &&
                        match.bankIndex == this->getActiveBank();
        // Only mark dirty if the value of the active bank changed
        values[match.bankIndex] = match.value;
        return newdirty;
    }

    void handleUpdate(typename Matcher::Result match) override {
        dirty |= handleUpdateImpl(match);
    }

  public:
    /// @name Data access
    /// @{

    /// Get the most recent MIDI value that was received for the active bank.
    uint8_t getValue() const override { return values[this->getActiveBank()]; }
    /// Get the most recent MIDI value that was received for the given bank.
    uint8_t getValue(uint8_t bank) const { return values[bank]; }

    /// @}

    /// Reset all values to zero.
    void reset() override {
        values = {{}};
        dirty = true;
    }

  protected:
    void onBankSettingChange() override { dirty = true; }

  private:
    AH::Array<uint8_t, BankSize> values = {{}};
};

/// Class that listens for MIDI Note events on a single address and
/// saves their value. This version listens accross multiple banks.
/// @ingroup    BankableMIDIInputElements
template <uint8_t BankSize>
using NoteValue = NoteCCKPValue<MIDIMessageType::NoteOn, BankSize>;

/// Class that listens for MIDI Control Change events on a single address and
/// saves their value. This version listens accross multiple banks.
/// @ingroup    BankableMIDIInputElements
template <uint8_t BankSize>
using CCValue = NoteCCKPValue<MIDIMessageType::ControlChange, BankSize>;

/// Class that listens for MIDI Key Pressure events on a single address and
/// saves their value. This version listens accross multiple banks.
/// @ingroup    BankableMIDIInputElements
template <uint8_t BankSize>
using KPValue = NoteCCKPValue<MIDIMessageType::KeyPressure, BankSize>;

} // namespace Bankable

END_CS_NAMESPACE
