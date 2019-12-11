#pragma once

#include <Banks/BankableMIDIInput.hpp>
#include <MIDI_Inputs/MIDIInputElementCC.hpp>
#include <MIDI_Inputs/MIDIInputElementNote.hpp>

BEGIN_CS_NAMESPACE

class INoteCCValue {
  protected:
    INoteCCValue(uint8_t rangeLength) : rangeLength{rangeLength} {}

  public:
    uint8_t length() const { return rangeLength; }
    virtual uint8_t getValue(uint8_t index) const = 0;
    uint8_t getValue() const { return getValue(0); }

  private:
    uint8_t rangeLength;
};

class NoteCCRangeEmptyCallback {
  public:
    void begin(const INoteCCValue &) {}
    void update(const INoteCCValue &, uint8_t) {}
    void updateAll(const INoteCCValue &) {}
};

struct SimpleNoteCCValueCallback {
  protected:
    SimpleNoteCCValueCallback() = default;

  public:
    virtual void begin(const INoteCCValue &) {}
    virtual void update(const INoteCCValue &, uint8_t) = 0;
    virtual void updateAll(const INoteCCValue &noteccval) {
        for (uint8_t i = 0; i < noteccval.length(); ++i)
            update(noteccval, i);
    }
};

// -------------------------------------------------------------------------- //

template <class MIDIInput_t, uint8_t RangeLen, uint8_t NumBanks, class Callback>
class NoteCCRange : public MIDIInput_t, public INoteCCValue {
  public:
    NoteCCRange(MIDICNChannelAddress address, const Callback &callback)
        : MIDIInput_t{address}, INoteCCValue{RangeLen}, callback(callback) {}

    /// @todo   check index bounds
    uint8_t getValue(uint8_t index) const override {
        return values[getSelection()][index];
    }
    using INoteCCValue::getValue;

    /// Initialize
    void begin() override { callback.begin(*this); }
    /// Reset all values to zero
    void reset() override {
        values = {{}};
        callback.updateAll(*this);
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

    Array<Array<uint8_t, RangeLen>, NumBanks> values = {{}};

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

template <uint8_t RangeLen, class Callback = NoteCCRangeEmptyCallback>
using GenericNoteRange =
    GenericNoteCCRange<MIDIInputElementNote, RangeLen, Callback>;

template <uint8_t RangeLen, class Callback = NoteCCRangeEmptyCallback>
using GenericCCRange =
    GenericNoteCCRange<MIDIInputElementCC, RangeLen, Callback>;

template <class Callback = NoteCCRangeEmptyCallback>
using GenericNoteValue = GenericNoteCCRange<MIDIInputElementNote, 1, Callback>;

template <class Callback = NoteCCRangeEmptyCallback>
using GenericCCValue = GenericNoteCCRange<MIDIInputElementCC, 1, Callback>;

/**
 * @brief   MIDI Input Element that listens to a range of notes.
 * 
 * @ingroup MIDIInputElements
 * @tparam  RangeLen 
 *          The length of the range of notes to listen to.
 */
template <uint8_t RangeLen>
class NoteRange : public GenericNoteRange<RangeLen> {
  public:
    NoteRange(MIDICNChannelAddress address)
        : GenericNoteRange<RangeLen>{address, {}} {}
};

class NoteValue : public GenericNoteValue<> {
  public:
    NoteValue(MIDICNChannelAddress address) : GenericNoteValue<>{address, {}} {}
};
using MIDINote[[deprecated("Use NoteValue instead")]] = NoteValue;

template <uint8_t RangeLen>
class CCRange : public GenericCCRange<RangeLen> {
  public:
    CCRange(MIDICNChannelAddress address)
        : GenericCCRange<RangeLen>{address, {}} {}
};

class CCValue : public GenericCCValue<> {
  public:
    CCValue(MIDICNChannelAddress address) : GenericCCValue<>{address, {}} {}
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

    void onBankSettingChange() override { this->callback.updateAll(*this); }
};

template <uint8_t RangeLen, uint8_t NumBanks,
          class Callback = NoteCCRangeEmptyCallback>
using GenericNoteRange =
    GenericNoteCCRange<MIDIInputElementNote, RangeLen, NumBanks, Callback>;

template <uint8_t RangeLen, uint8_t NumBanks,
          class Callback = NoteCCRangeEmptyCallback>
using GenericCCRange =
    GenericNoteCCRange<MIDIInputElementCC, RangeLen, NumBanks, Callback>;

template <uint8_t NumBanks, class Callback = NoteCCRangeEmptyCallback>
using GenericNoteValue =
    GenericNoteCCRange<MIDIInputElementNote, 1, NumBanks, Callback>;

template <uint8_t NumBanks, class Callback = NoteCCRangeEmptyCallback>
using GenericCCValue =
    GenericNoteCCRange<MIDIInputElementCC, 1, NumBanks, Callback>;

template <uint8_t RangeLen, uint8_t NumBanks>
class NoteRange : public GenericNoteRange<RangeLen, NumBanks> {
  public:
    NoteRange(BankConfig<NumBanks> config, MIDICNChannelAddress address)
        : GenericNoteRange<RangeLen, NumBanks>{config, address, {}} {}
};

template <uint8_t NumBanks>
class NoteValue : public GenericNoteValue<NumBanks> {
  public:
    NoteValue(BankConfig<NumBanks> config, MIDICNChannelAddress address)
        : GenericNoteValue<NumBanks>{config, address, {}} {}
};
template <uint8_t NumBanks>
using MIDINote[[deprecated("Use NoteValue instead")]] = NoteValue<NumBanks>;

template <uint8_t RangeLen, uint8_t NumBanks>
class CCRange : public GenericCCRange<RangeLen, NumBanks> {
  public:
    CCRange(BankConfig<NumBanks> config, MIDICNChannelAddress address)
        : GenericCCRange<RangeLen, NumBanks>{config, address, {}} {}
};

template <uint8_t NumBanks>
class CCValue : public GenericCCValue<NumBanks> {
  public:
    CCValue(BankConfig<NumBanks> config, MIDICNChannelAddress address)
        : GenericCCValue<NumBanks>{config, address, {}} {}
};

} // namespace Bankable

END_CS_NAMESPACE