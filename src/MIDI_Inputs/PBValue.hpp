#pragma once

#include "InterfaceMIDIInputElements.hpp"
#include "MIDIInputElementMatchers.hpp"

BEGIN_CS_NAMESPACE

// -------------------------------------------------------------------------- //

/// Class that listens for MIDI Pitch Bend events on a single address and saves
/// their value.
/// @ingroup    MIDIInputElements
class PBValue : public MatchingMIDIInputElement<MIDIMessageType::PitchBend,
                                                PitchBendMIDIMatcher>,
                public Interfaces::IValue14 {
  public:
    using Matcher = PitchBendMIDIMatcher;

    /// @param  address
    ///         The address to listen to.
    PBValue(MIDIChannelCable address) : MatchingMIDIInputElement(address) {}

  protected:
    void handleUpdate(typename Matcher::Result match) override {
        dirty |= value != match.value;
        value = match.value;
    }

  public:
    /// @name Data access
    /// @{

    /// Get the most recent MIDI value that was received.
    uint16_t getValue() const override { return value; }

    /// @}

    /// Reset all values to zero.
    void reset() override {
        value = 0;
        dirty = true;
    }

  private:
    uint16_t value = 0;
};

// -------------------------------------------------------------------------- //

namespace Bankable {

/// Class that listens for MIDI Pitch Bend events on a single address and saves
/// their value. This version listens accross multiple banks.
///
/// @tparam BankSize
///         The number of banks.
/// @ingroup    MIDIInputElements
template <uint8_t BankSize>
class PBValue
    : public BankableMatchingMIDIInputElement<
          MIDIMessageType::PitchBend, BankablePitchBendMIDIMatcher<BankSize>>,
      public Interfaces::IValue14 {
  public:
    constexpr static auto MessageType = MIDIMessageType::PitchBend;
    using Matcher = BankablePitchBendMIDIMatcher<BankSize>;
    using Parent = BankableMatchingMIDIInputElement<MessageType, Matcher>;

    /// @param  config
    ///         The bank configuration to use.
    /// @param  address
    ///         The base address to listen to.
    PBValue(BankConfig<BankSize, BankType::ChangeChannel> config,
            MIDIChannelCable address)
        : Parent({config, address}) {}

  protected:
    void handleUpdate(typename Matcher::Result match) override {
        dirty |= values[match.bankIndex] != match.value &&
                 match.bankIndex == this->getActiveBank();
        // Only mark dirty if the value of the active bank changed
        values[match.bankIndex] = match.value;
    }

  public:
    /// @name Data access
    /// @{

    /// Get the most recent MIDI value that was received for the active bank.
    uint16_t getValue() const override { return values[this->getActiveBank()]; }
    /// Get the most recent MIDI value that was received for the given bank.
    uint16_t getValue(uint8_t bank) const { return values[bank]; }

    /// @}

    /// Reset all values to zero.
    void reset() override {
        values = {{}};
        dirty = true;
    }

  protected:
    void onBankSettingChange() override { dirty = true; }

  private:
    AH::Array<uint16_t, BankSize> values = {{}};
};

} // namespace Bankable

END_CS_NAMESPACE
