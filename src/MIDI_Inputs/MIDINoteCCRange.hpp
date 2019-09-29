#pragma once

#include <Banks/BankableMIDIInput.hpp>
#include <MIDI_Inputs/MIDIInputElementCC.hpp>
#include <MIDI_Inputs/MIDIInputElementNote.hpp>

BEGIN_CS_NAMESPACE

class MIDINoteCCRangeEmptyCallback {
  public:
    template <class T>
    void begin(const T &) {}
    template <class T>
    void update(const T &, uint8_t) {}
    template <class T>
    void update(const T &) {}
};

// -------------------------------------------------------------------------- //

template <class MIDIInput_t, uint8_t RangeLen,
          class Callback = MIDINoteCCRangeEmptyCallback>
class MIDINoteCCRange : public MIDIInput_t {
  public:
    MIDINoteCCRange(const MIDICNChannelAddress &address,
                    const Callback &callback = {})
        : MIDIInput_t{address}, callback{callback} {}

    /// @todo   check index bounds
    uint8_t getValue(uint8_t index = 0) const { return values[index]; }

    void begin() override { callback.begin(*this); }

    void reset() override {
        values = {};
        callback.update(*this);
    }

  private:
    bool match(const MIDICNChannelAddress &target) const override {
        return MIDICNChannelAddress::matchAddressInRange( //
            target, this->address, RangeLen);
    }

    bool updateImpl(const ChannelMessageMatcher &midimsg,
                    const MIDICNChannelAddress &target) override {
        uint8_t index = getRangeIndex(target);
        values[index] = getValueFromMIDIMessage(midimsg);
        callback.update(*this, index);
        return true;
    }

    static uint8_t
    getValueFromMIDIMessage(const ChannelMessageMatcher &midimsg) {
        return midimsg.type == NOTE_OFF ? 0 : midimsg.data2;
    }

    uint8_t getRangeIndex(const MIDICNChannelAddress &target) const {
        return target.getAddress() - this->address.getAddress();
    }

    Array<uint8_t, RangeLen> values = {};

  public:
    Callback callback;
    constexpr static uint8_t length() { return RangeLen; }
};

// -------------------------------------------------------------------------- //

template <uint8_t RangeLen>
using NoteRange = MIDINoteCCRange<MIDIInputElementNote, RangeLen>;
using NoteValue = MIDINoteCCRange<MIDIInputElementNote, 1>;

template <uint8_t RangeLen>
using CCRange = MIDINoteCCRange<MIDIInputElementCC, RangeLen>;
using CCValue = MIDINoteCCRange<MIDIInputElementCC, 1>;

// -------------------------------------------------------------------------- //

#if 1

namespace Bankable {

/// @tparam RangeLen
///         The length of the range.
/// @tparam BankSize
///         The size of the bank.
template <class MIDIInput_t, uint8_t RangeLen, uint8_t BankSize,
          class Callback = MIDINoteCCRangeEmptyCallback>
class MIDINoteCCRange : public MIDIInput_t, public BankableMIDIInput<BankSize> {
  public:
    MIDINoteCCRange(const BankConfig<BankSize> &config,
                    const MIDICNChannelAddress &address,
                    const Callback &callback = {})
        : MIDIInput_t{address},
          BankableMIDIInput<BankSize>{config}, callback{callback} {}

    /// @todo   check index bounds
    uint8_t getValue(uint8_t index = 0) const {
        // getSelection will always be less than N
        return values[this->getSelection()][index];
    }

    void begin() override { callback.begin(*this); }

    void reset() override {
        values = {};
        callback.update(*this);
    }

  private:
    bool updateImpl(const ChannelMessageMatcher &midimsg,
                    const MIDICNChannelAddress &target) override {
        uint8_t bankIndex = this->getBankIndex(target, this->address);
        uint8_t rangeIndex = this->getRangeIndex(target);
        uint8_t value = getValueFromMIDIMessage(midimsg);
        values[bankIndex][rangeIndex] = value;
        if (bankIndex == this->getSelection())
            callback.update(*this, rangeIndex);
        return true;
    }

    bool match(const MIDICNChannelAddress &target) const override {
        return BankableMIDIInput<BankSize>::matchBankableAddressInRange(
            target, this->address, RangeLen);
    }

    void onBankSettingChange() override { callback.update(*this); }

    static uint8_t
    getValueFromMIDIMessage(const ChannelMessageMatcher &midimsg) {
        return midimsg.type == NOTE_OFF ? 0 : midimsg.data2;
    }

    uint8_t getRangeIndex(const MIDICNChannelAddress &target) const {
        return target.getAddress() - this->address.getAddress();
    }

    Array<Array<uint8_t, RangeLen>, BankSize> values = {};

  public:
    Callback callback;
    constexpr static uint8_t length() { return RangeLen; }
};

template <uint8_t RangeLen, uint8_t BankSize>
using NoteRange = MIDINoteCCRange<MIDIInputElementNote, RangeLen, BankSize>;
template <uint8_t BankSize>
using NoteValue = MIDINoteCCRange<MIDIInputElementNote, 1, BankSize>;

template <uint8_t RangeLen, uint8_t BankSize>
using CCRange = MIDINoteCCRange<MIDIInputElementCC, RangeLen, BankSize>;
template <uint8_t BankSize>
using CCValue = MIDINoteCCRange<MIDIInputElementCC, 1, BankSize>;

} // namespace Bankable

#endif

END_CS_NAMESPACE