#pragma once

#include <Banks/BankableMIDIInput.hpp>
#include <MIDI_Inputs/MIDIInputElementCC.hpp>
#include <MIDI_Inputs/MIDIInputElementNote.hpp>

BEGIN_CS_NAMESPACE

class NoteCCRangeEmptyCallback {
  public:
    template <class T>
    void begin(const T &) {}
    template <class T>
    void update(const T &, uint8_t) {}
    template <class T>
    void update(const T &) {}
};

// -------------------------------------------------------------------------- //

class INoteCCValue {
  protected:
    INoteCCValue() = default;

  public:
    virtual uint8_t getValue(uint8_t index) const = 0;
    uint8_t getValue() const { return getValue(0); }
};

template <class MIDIInput_t, uint8_t RangeLen, uint8_t NumBanks, class Callback>
class NoteCCRange : public MIDIInput_t, public INoteCCValue {
  public:
    NoteCCRange(MIDICNChannelAddress address, const Callback &callback)
        : MIDIInput_t{address}, callback{callback} {}

    /// @todo   check index bounds
    uint8_t getValue(uint8_t index = 0) const override {
        return values[getSelection()][index];
    }
    using INoteCCValue::getValue;

    /// Initialize
    void begin() override { callback.begin(*this); }
    /// Reset all values to zero
    void reset() override {
        values = {};
        callback.update(*this);
    }

  private:
    bool updateImpl(const ChannelMessageMatcher &midimsg,
                    const MIDICNChannelAddress &target) override {
        uint8_t bankIndex = this->getBankIndex(target);
        uint8_t rangeIndex = this->getRangeIndex(target);
        uint8_t value = getValueFromMIDIMessage(midimsg);
        values[bankIndex][rangeIndex] = value;
        if (bankIndex == this->getSelection())
            callback.update(*this, rangeIndex);
        return true;
    }

    static uint8_t
    getValueFromMIDIMessage(const ChannelMessageMatcher &midimsg) {
        return midimsg.type == NOTE_OFF ? 0 : midimsg.data2;
    }

    /// Get the active bank selection
    virtual uint8_t getSelection() const { return 0; }

    /// Get the bank index from a MIDI address
    virtual setting_t getBankIndex(MIDICNChannelAddress target) const {
        (void)target;
        return 0;
    }

    virtual uint8_t getRangeIndex(MIDICNChannelAddress target) const {
        return target.getAddress() - this->address.getAddress();
    }

    Array<Array<uint8_t, RangeLen>, NumBanks> values = {};

  public:
    Callback callback;
    constexpr static uint8_t length() { return RangeLen; }
};

// -------------------------------------------------------------------------- //

template <class MIDIInput_t, uint8_t RangeLen,
          class Callback = NoteCCRangeEmptyCallback>
class GenericNoteCCRange
    : public NoteCCRange<MIDIInput_t, RangeLen, 1, Callback> {
  public:
    GenericNoteCCRange(MIDICNChannelAddress address, const Callback &callback)
        : NoteCCRange<MIDIInput_t, RangeLen, 1, Callback>{address, callback} {}

  private:
    /// Check if the address of the incoming MIDI message is within the range
    /// of addresses of this element.
    bool match(const MIDICNChannelAddress &target) const override {
        return MIDICNChannelAddress::matchAddressInRange( //
            target, this->address, RangeLen);
    }
};

template <uint8_t RangeLen>
class NoteRange : public GenericNoteCCRange<MIDIInputElementNote, RangeLen> {
  public:
    NoteRange(MIDICNChannelAddress address)
        : GenericNoteCCRange<MIDIInputElementNote, RangeLen>{address, {}} {}
};

class NoteValue : public GenericNoteCCRange<MIDIInputElementNote, 1> {
  public:
    NoteValue(MIDICNChannelAddress address)
        : GenericNoteCCRange<MIDIInputElementNote, 1>{address, {}} {}
};
using MIDINote __attribute__((deprecated)) = NoteValue;

template <uint8_t RangeLen>
class CCRange : public GenericNoteCCRange<MIDIInputElementCC, RangeLen> {
  public:
    CCRange(MIDICNChannelAddress address)
        : GenericNoteCCRange<MIDIInputElementCC, RangeLen>{address, {}} {}
};

class CCValue : public GenericNoteCCRange<MIDIInputElementCC, 1> {
  public:
    CCValue(MIDICNChannelAddress address)
        : GenericNoteCCRange<MIDIInputElementCC, 1>{address, {}} {}
};

// -------------------------------------------------------------------------- //

namespace Bankable {

/// @tparam RangeLen
///         The length of the range.
/// @tparam NumBanks
///         The size of the bank.
template <class MIDIInput_t, uint8_t RangeLen, uint8_t NumBanks,
          class Callback = NoteCCRangeEmptyCallback>
class GenericNoteCCRange
    : public NoteCCRange<MIDIInput_t, RangeLen, NumBanks, Callback>,
      public BankableMIDIInput<NumBanks> {
  public:
    GenericNoteCCRange(BankConfig<NumBanks> config,
                       MIDICNChannelAddress address,
                       const Callback &callback)
        : NoteCCRange<MIDIInput_t, RangeLen, NumBanks, Callback>{
            address,
            callback,
        }, BankableMIDIInput<NumBanks>{config} {}

  private:
    /// Check if the address of the incoming MIDI message is within the range
    /// of addresses and in one of the banks of this element.
    bool match(const MIDICNChannelAddress &target) const override {
        return BankableMIDIInput<NumBanks>::matchBankableAddressInRange(
            target, this->address, RangeLen);
    }

    setting_t getSelection() const override {
        return BankableMIDIInput<NumBanks>::getSelection();
    };

    uint8_t getBankIndex(MIDICNChannelAddress target) const override {
        return BankableMIDIInput<NumBanks>::getBankIndex(target, this->address);
    }

    uint8_t getRangeIndex(MIDICNChannelAddress target) const override {
        return BankableMIDIInput<NumBanks>::getRangeIndex(target,
                                                          this->address);
    }

    void onBankSettingChange() override { this->callback.update(*this); }
};

template <uint8_t RangeLen, uint8_t NumBanks>
class NoteRange
    : public GenericNoteCCRange<MIDIInputElementNote, RangeLen, NumBanks> {
  public:
    NoteRange(BankConfig<NumBanks> config, MIDICNChannelAddress address)
        : GenericNoteCCRange<MIDIInputElementNote, RangeLen, NumBanks>{
              config, address, {}} {}
};

template <uint8_t NumBanks>
class NoteValue : public GenericNoteCCRange<MIDIInputElementNote, 1, NumBanks> {
  public:
    NoteValue(BankConfig<NumBanks> config, MIDICNChannelAddress address)
        : GenericNoteCCRange<MIDIInputElementNote, 1, NumBanks>{
              config, address, {}} {}
};
template <uint8_t NumBanks>
using MIDINote __attribute__((deprecated)) = NoteValue<NumBanks>;

template <uint8_t RangeLen, uint8_t NumBanks>
class CCRange
    : public GenericNoteCCRange<MIDIInputElementCC, RangeLen, NumBanks> {
  public:
    CCRange(BankConfig<NumBanks> config, MIDICNChannelAddress address)
        : GenericNoteCCRange<MIDIInputElementCC, RangeLen, NumBanks>{
              config, address, {}} {}
};

template <uint8_t NumBanks>
class CCValue : public GenericNoteCCRange<MIDIInputElementCC, 1, NumBanks> {
  public:
    CCValue(BankConfig<NumBanks> config, MIDICNChannelAddress address)
        : GenericNoteCCRange<MIDIInputElementCC, 1, NumBanks>{
              config, address, {}} {}
};

} // namespace Bankable

END_CS_NAMESPACE