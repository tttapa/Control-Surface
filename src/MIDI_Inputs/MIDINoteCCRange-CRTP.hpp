#pragma once

#include <Banks/BankableMIDIInput.hpp>
#include <MIDI_Inputs/MIDIInputElementCC.hpp>
#include <MIDI_Inputs/MIDIInputElementNote.hpp>

BEGIN_CS_NAMESPACE

#define CRTP(derived) (*static_cast<derived *>(this))

template <class MIDIInput_t, uint8_t L, class Callback>
class MIDINoteCCRange_Base : public MIDIInput_t {
  public:
    MIDINoteCCRange_Base(const MIDICNChannelAddress &address)
        : MIDIInput_t{address} {}

    /// @todo: check index bounds
    uint8_t getValue(uint8_t index = 0) const { return values[index]; }

    void begin() override { CRTP(Callback).begin(); }

    void reset() override {
        values = {};
        CRTP(Callback).update();
    }

  private:
    bool match(const MIDICNChannelAddress &target) const override {
        return MIDICNChannelAddress::matchAddressInRange( //
            target, this->address, L);
    }

    bool updateImpl(const ChannelMessageMatcher &midimsg,
                    const MIDICNChannelAddress &target) override {
        uint8_t index = getRangeIndex(target);
        values[index] = getValueFromMIDIMessage(midimsg);
        CRTP(Callback).update(index);
        return true;
    }

    static uint8_t
    getValueFromMIDIMessage(const ChannelMessageMatcher &midimsg) {
        return midimsg.type == NOTE_OFF ? 0 : midimsg.data2;
    }

    uint8_t getRangeIndex(const MIDICNChannelAddress &target) const {
        return target.getAddress() - this->address.getAddress();
    }

    Array<uint8_t, L> values = {};
};

template <class MIDIInput_t, uint8_t L>
class MIDINoteCCRange
    : public MIDINoteCCRange_Base<MIDIInput_t, L,
                                  MIDINoteCCRange<MIDIInput_t, L>> {
  public:
    MIDINoteCCRange(const MIDICNChannelAddress &address)
        : MIDINoteCCRange_Base<MIDIInput_t, L, MIDINoteCCRange<MIDIInput_t, L>>{
              address} {}

  private:
    void begin() {}
    void update(UNUSED_PARAM uint8_t index) {}
    void update() {}

    friend class MIDINoteCCRange_Base<MIDIInput_t, L,
                                      MIDINoteCCRange<MIDIInput_t, L>>;
};

template <uint8_t L>
using NoteRange = MIDINoteCCRange<MIDIInputElementNote, L>;
using NoteValue = MIDINoteCCRange<MIDIInputElementNote, 1>;

template <uint8_t L>
using CCRange = MIDINoteCCRange<MIDIInputElementCC, L>;
using CCValue = MIDINoteCCRange<MIDIInputElementCC, 1>;

// -------------------------------------------------------------------------- //

#if 0

namespace Bankable {

template <class MIDIInput_t, uint8_t L, uint8_t N, class Callback>
class MIDINoteCCRange;

template <class MIDIInput_t, uint8_t L, uint8_t N>
struct MIDINoteCCRange
    : public MIDINoteCCRange<MIDIInput_t, L, N> {
    void begin() {}
    void update(uint8_t index) {}
    void update() {}
};

/// @tparam L
///         The length of the range.
/// @tparam N
///         The size of the bank.
/// @todo
template <class MIDIInput_t, uint8_t L, uint8_t N,
          class Callback = MIDINoteCCRange<MIDIInput_t, L, N>>
class MIDINoteCCRange : public MIDINoteCCRange_Base<MIDIInput_t>,
                        public BankableMIDIInput<N> {
  public:
    MIDINoteCCRange(const BankConfig<N> &config,
                    const MIDICNChannelAddress &address)
        : MIDINoteCCRange_Base<MIDIInput_t>(address), //
          BankableMIDIInput<N>(config) {}

    /// @todo: check index bounds
    uint8_t getValue(uint8_t index = 0) const {
        // getSelection will always be less than N
        return values[this->getSelection()][index];
    }

    void begin() override { callback.begin(); }

    void reset() override {
        values = {};
        callback.update();
    }

  private:
    bool updateImpl(const ChannelMessageMatcher &midimsg,
                    const MIDICNChannelAddress &target) override {
        uint8_t bankindex = this->getBankIndex(target, address);
        uint8_t rangeIndex = this->getRangeIndex(target);
        uint8_t value = getValueFromMIDIMessage(midimsg);
        values[bankindex][rangeIndex] = value;
        if (bankindex == this->getSelection())
            CRTP(Callback).update(rangeIndex);
        return true;
    }

    bool match(const MIDICNChannelAddress &target) const override {
        return BankableMIDIInput<N>::matchBankableAddressInRange( //
            target, address, L);
    }

    void onBankSettingChange() override {
        CRTP(Callback).update();
    }

    Array<Array<uint8_t, L>, N> values = {};
};

template <uint8_t L, uint8_t N>
using NoteRange = MIDINoteCCRange<MIDIInputElementNote, L, N>;
template <uint8_t N>
using NoteValue = MIDINoteCCRange<MIDIInputElementNote, 1, N>;

template <uint8_t L, uint8_t N>
using CCRange = MIDINoteCCRange<MIDIInputElementCC, L, N>;
template <uint8_t L, uint8_t N>
using CCValue = MIDINoteCCRange<MIDIInputElementCC, 1, N>;

} // namespace Bankable

#endif

END_CS_NAMESPACE