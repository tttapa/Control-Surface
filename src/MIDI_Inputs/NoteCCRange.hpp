#pragma once

#include <Banks/BankableMIDIInput.hpp>
#include <MIDI_Inputs/MIDIInputElementCC.hpp>
#include <MIDI_Inputs/MIDIInputElementNote.hpp>

BEGIN_CS_NAMESPACE

/**
 * @brief   Interface for NoteCCValue objects: provides getters for the 
 *          velocity or controller values.
 */
class INoteCCValue {
  protected:
    INoteCCValue(uint8_t rangeLength) : rangeLength{rangeLength} {}

  public:
    /// Get the length of the range of note/CC addresses.
    uint8_t length() const { return rangeLength; }
    /// Get the velocity or controller value for the given index in the range.
    virtual uint8_t getValue(uint8_t index) const = 0;
    /// Get the velocity or controller value of the first or only note or
    /// controller.
    uint8_t getValue() const { return getValue(0); }

  private:
    uint8_t rangeLength;
};

/**
 * @brief   A callback for NoteCCRange that doesn't do anything.
 */
class NoteCCRangeEmptyCallback {
  public:
    void begin(const INoteCCValue &) {}
    void update(const INoteCCValue &, uint8_t) {}
    void updateAll(const INoteCCValue &) {}
};

/**
 * @brief   A callback for NoteCCRange with an action that can be implemented 
 *          by the user.
 * 
 * Provides default implementations for `begin` and `updateAll`.
 */
struct SimpleNoteCCValueCallback {
  protected:
    SimpleNoteCCValueCallback() = default;

  public:
    /// Initialize: called once
    virtual void begin(const INoteCCValue &) {}
    /// Update the given index: called when a new message is received for this
    /// index
    virtual void update(const INoteCCValue &noteccval, uint8_t index) = 0;
    /// Update all values: called when a bank change causes all values to
    /// (possibly) change, or when the entire range is reset to zero.
    virtual void updateAll(const INoteCCValue &noteccval) {
        for (uint8_t i = 0; i < noteccval.length(); ++i)
            update(noteccval, i);
    }
};

// -------------------------------------------------------------------------- //

/** 
 * @brief   Base class for all other classes that listen for incoming MIDI Note 
 *          or Control Change messages and saves their values.
 * 
 * Can listen to a range of addresses or a single address.
 */
template <class MIDIInput_t, uint8_t RangeLen, uint8_t NumBanks, class Callback>
class NoteCCRange : public MIDIInput_t, public INoteCCValue {
  public:
    NoteCCRange(MIDIAddress address, const Callback &callback)
        : MIDIInput_t{address}, INoteCCValue{RangeLen}, callback(callback) {}

    /// @todo   check index bounds
    uint8_t getValue(uint8_t index) const final override {
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
    // Called when a MIDI message comes in, and if that message has been matched
    // by the `match` method.
    bool updateImpl(const ChannelMessageMatcher &midimsg,
                    const MIDIAddress &target) override {
        // Compute the offsets/indices in the range and the bank.
        // E.g. if the start address of the range is 10 and the incoming message
        // is on address 15, the range index will be 5 = 15 - 10.
        // Very similar for banks, but in that case there are 3 possible 
        // addresses (note/controller number, MIDI channel and MIDI cable).
        uint8_t bankIndex = this->getBankIndex(target);
        uint8_t rangeIndex = this->getRangeIndex(target);
        // extract the velocity or controller value from the message
        uint8_t value = getValueFromMIDIMessage(midimsg);
        // save the value
        values[bankIndex][rangeIndex] = value;
        // if the bank that the message belongs to is the active bank,
        // update the display
        if (bankIndex == this->getSelection())
            callback.update(*this, rangeIndex);
        // event was handled successfully
        return true;
    }

    /// Extract the "value" from a MIDI Note or Control Change message.
    /// For Note On and Control Change, this is simply the second data byte,
    /// for Note Off, it's zero.
    static uint8_t
    getValueFromMIDIMessage(const ChannelMessageMatcher &midimsg) {
        return midimsg.type == MIDIMessageType::NOTE_OFF ? 0 : midimsg.data2;
    }

    /// Get the active bank selection.
    virtual uint8_t getSelection() const { return 0; }

    /// Get the bank index from a MIDI address.
    virtual setting_t getBankIndex(MIDIAddress target) const {
        // Default implementation for non-bankable version (bank is always 0)
        (void)target;
        return 0;
    }

    /// Get the index of the given MIDI address in the range
    virtual uint8_t getRangeIndex(MIDIAddress target) const {
        // Default implementation for non-bankable version (base address of the 
        // range is fixed)
        return target.getAddress() - this->address.getAddress();
    }

    /// A 2D array for saving all values of the range, for all banks.
    Array<Array<uint8_t, RangeLen>, NumBanks> values = {{}};

  public:
    /// Callback that is called when a value in the active bank changes.
    Callback callback;
    /// Get the length of the range.
    constexpr static uint8_t length() { return RangeLen; }
};

// -------------------------------------------------------------------------- //

template <class MIDIInput_t, uint8_t RangeLen,
          class Callback = NoteCCRangeEmptyCallback>
class GenericNoteCCRange
    : public NoteCCRange<MIDIInput_t, RangeLen, 1, Callback> {
  public:
    GenericNoteCCRange(MIDIAddress address, const Callback &callback)
        : NoteCCRange<MIDIInput_t, RangeLen, 1, Callback>{address, callback} {}

  private:
    /// Check if the address of the incoming MIDI message is within the range
    /// of addresses of this element.
    bool match(const MIDIAddress &target) const override {
        return MIDIAddress::matchAddressInRange( //
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
 * @brief   MIDI Input Element that listens to a range of notes and saves their
 *          velocity values.
 * 
 * @ingroup MIDIInputElements
 * @tparam  RangeLen 
 *          The length of the range of notes to listen to.
 */
template <uint8_t RangeLen>
class NoteRange : public GenericNoteRange<RangeLen> {
  public:
    NoteRange(MIDIAddress address)
        : GenericNoteRange<RangeLen>{address, {}} {}
};

/**
 * @brief   MIDI Input Element that listens to a single note and saves its 
 *          velocity value.
 * 
 * @ingroup MIDIInputElements
 */
class NoteValue : public GenericNoteValue<> {
  public:
    NoteValue(MIDIAddress address) : GenericNoteValue<>{address, {}} {}
};
using MIDINote[[deprecated("Use NoteValue instead")]] = NoteValue;

/**
 * @brief   MIDI Input Element that listens to a range of controllers
 *          and saves their values.
 * 
 * @ingroup MIDIInputElements
 * @tparam  RangeLen 
 *          The length of the range of controllers to listen to.
 */
template <uint8_t RangeLen>
class CCRange : public GenericCCRange<RangeLen> {
  public:
    CCRange(MIDIAddress address)
        : GenericCCRange<RangeLen>{address, {}} {}
};

/**
 * @brief   MIDI Input Element that listens to a single controller and saves its 
 *          value.
 * 
 * @ingroup MIDIInputElements
 */
class CCValue : public GenericCCValue<> {
  public:
    CCValue(MIDIAddress address) : GenericCCValue<>{address, {}} {}
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
                       MIDIAddress address,
                       const Callback &callback)
        : NoteCCRange<MIDIInput_t, RangeLen, NumBanks, Callback>{
            address,
            callback,
        }, BankableMIDIInput<NumBanks>{config} {}

  private:
    /// Check if the address of the incoming MIDI message is within the range
    /// of addresses and in one of the banks of this element.
    bool match(const MIDIAddress &target) const override {
        return BankableMIDIInput<NumBanks>::matchBankableAddressInRange(
            target, this->address, RangeLen);
    }

    setting_t getSelection() const override {
        return BankableMIDIInput<NumBanks>::getSelection();
    };

    uint8_t getBankIndex(MIDIAddress target) const override {
        return BankableMIDIInput<NumBanks>::getBankIndex(target, this->address);
    }

    uint8_t getRangeIndex(MIDIAddress target) const override {
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

/**
 * @brief   MIDI Input Element that listens to a range of notes and saves their
 *          velocity values.
 * 
 * @ingroup BankableMIDIInputElements
 * @tparam  RangeLen 
 *          The length of the range of notes to listen to.
 * @tparam  NumBanks 
 *          The size of the bank.
 */
template <uint8_t RangeLen, uint8_t NumBanks>
class NoteRange : public GenericNoteRange<RangeLen, NumBanks> {
  public:
    NoteRange(BankConfig<NumBanks> config, MIDIAddress address)
        : GenericNoteRange<RangeLen, NumBanks>{config, address, {}} {}
};

/**
 * @brief   MIDI Input Element that listens to a single note and saves its
 *          value.
 * 
 * @ingroup BankableMIDIInputElements
 * @tparam  NumBanks 
 *          The size of the bank.
 */
template <uint8_t NumBanks>
class NoteValue : public GenericNoteValue<NumBanks> {
  public:
    NoteValue(BankConfig<NumBanks> config, MIDIAddress address)
        : GenericNoteValue<NumBanks>{config, address, {}} {}
};

/// Deprecated.
/// @see Bankable::NoteValue
template <uint8_t NumBanks>
using MIDINote[[deprecated("Use NoteValue instead")]] = NoteValue<NumBanks>;

/**
 * @brief   MIDI Input Element that listens to a range of controllers and saves 
 *          their values.
 * 
 * @ingroup BankableMIDIInputElements
 * @tparam  RangeLen 
 *          The length of the range of controllers to listen to.
 * @tparam  NumBanks 
 *          The size of the bank.
 */
template <uint8_t RangeLen, uint8_t NumBanks>
class CCRange : public GenericCCRange<RangeLen, NumBanks> {
  public:
    CCRange(BankConfig<NumBanks> config, MIDIAddress address)
        : GenericCCRange<RangeLen, NumBanks>{config, address, {}} {}
};

/**
 * @brief   MIDI Input Element that listens to a single controller and saves its
 *          value.
 * 
 * @ingroup BankableMIDIInputElements
 * @tparam  NumBanks 
 *          The size of the bank.
 */
template <uint8_t NumBanks>
class CCValue : public GenericCCValue<NumBanks> {
  public:
    CCValue(BankConfig<NumBanks> config, MIDIAddress address)
        : GenericCCValue<NumBanks>{config, address, {}} {}
};

} // namespace Bankable

END_CS_NAMESPACE