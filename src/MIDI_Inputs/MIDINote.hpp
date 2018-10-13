#pragma once

#include "MIDIInputElementNote.hpp"

/** An abstract base class for MIDI note input. */
class MIDINote_Base : public MIDIInputElementNote {
  protected:
    MIDINote_Base(); // unused, only for virtual inheritance
    MIDINote_Base(const MIDICNChannelAddress &address)
        : MIDIInputElementNote(address) {}

  public:
    virtual uint8_t getValue() const = 0;
    bool getState() const { return getValue() >= NOTE_VELOCITY_THRESHOLD; }

  protected:
    uint8_t getValueFromMIDIMessage(const MIDI_message_matcher &midimsg) {
        return midimsg.type == NOTE_ON ? midimsg.data2 : 0;
    }
};

// -------------------------------------------------------------------------- //

/** A class for MIDI note input. */
class MIDINote : virtual public MIDINote_Base {
  public:
    MIDINote(const MIDICNChannelAddress &address) : MIDINote_Base(address) {}

    uint8_t getValue() const override { return value; }

    void reset() override { value = 0; }

  private:
    bool updateImpl(const MIDI_message_matcher &midimsg,
                    const MIDICNChannelAddress &target) override {
        this->value = getValueFromMIDIMessage(midimsg);
        return true;
    }

    uint8_t value = 0;
};

// -------------------------------------------------------------------------- //

#include <Banks/BankableMIDIInput.hpp>

namespace Bankable {

/** A class for Bankable MIDI note input. */
template <uint8_t N>
class MIDINote : virtual public MIDINote_Base, public BankableMIDIInput<N> {
  public:
    MIDINote(const BankConfig<N> &config, const MIDICNChannelAddress &address)
        : MIDINote_Base(address), BankableMIDIInput<N>(config) {}

    uint8_t getValue() const override {
        // getSelection will always be less than N
        return values[this->getSelection()];
    }

    void reset() override {
        for (uint8_t &value : values)
            value = 0;
    }

  private:
    bool updateImpl(const MIDI_message_matcher &midimsg,
                    const MIDICNChannelAddress &target) override {
        uint8_t index = this->getIndex(target, address);
        uint8_t value = getValueFromMIDIMessage(midimsg);
        this->values[index] = value;
        return true;
    }

    bool match(const MIDICNChannelAddress &target) const override {
        return BankableMIDIInput<N>::matchBankable(target, address);
    }

    void onBankSettingChange() const override { display(); }

    uint8_t values[N] = {};
};

} // namespace Bankable