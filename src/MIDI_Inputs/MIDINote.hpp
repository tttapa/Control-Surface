#pragma once

#include "MIDIInputElementNote.hpp"

class MIDINote_Base : public MIDIInputElementNote {
  protected:
    MIDINote_Base(); // unused, only for virtual inheritance
    MIDINote_Base(uint8_t track, uint8_t channel = 1)
        : MIDIInputElementNote(channel, track) {}

  public:
    virtual uint8_t getValue() const = 0;

  protected:
    uint8_t getValueFromMIDIMessage(const MIDI_message_matcher &midimsg) {
        return midimsg.type == NOTE_ON ? midimsg.data2 : 0;
    }

  private:
    virtual bool updateImpl(const MIDI_message_matcher &midimsg) = 0;
};

// -------------------------------------------------------------------------- //

class MIDINote : public virtual MIDINote_Base {
  public:
    MIDINote(uint8_t track, uint8_t channel = 1)
        : MIDINote_Base(track, channel) {}

    uint8_t getValue() const override { return value; }

    void reset() override { value = 0; }

  private:
    bool updateImpl(const MIDI_message_matcher &midimsg) override {
        this->value = getValueFromMIDIMessage(midimsg);
        return true;
    }

    uint8_t value = 0;
};

// -------------------------------------------------------------------------- //

#include <Banks/BankableMIDIInputAddressable.hpp>

namespace Bankable {

template <uint8_t NUMBER_OF_BANKS>
class MIDINote : public virtual MIDINote_Base,
                   public BankableMIDIInputAddressable {
  public:
    MIDINote(uint8_t track, uint8_t channel = 1)
        : MIDINote_Base(track, channel) {}

    uint8_t getValue() const override {
        return values[getBankSetting() % NUMBER_OF_BANKS];
    }

    void reset() override {
        for (uint8_t &value : values)
            value = 0;
    }

  private:
    bool updateImpl(const MIDI_message_matcher &midimsg) override {
        uint8_t index = getIndex(midimsg.channel, midimsg.data1,
                                 getBaseChannel(), getBaseAddress());
        uint8_t value = getValueFromMIDIMessage(midimsg);
        this->values[index] = value;
        return true;
    }

    bool matchAddress(uint8_t targetAddress) const override {
        return BankableMIDIInputAddressable::matchAddress(
            targetAddress, getBaseAddress(), NUMBER_OF_BANKS);
    }

    bool matchChannel(uint8_t targetChannel) const override {
        return BankableMIDIInputAddressable::matchChannel(
            targetChannel, getBaseChannel(), NUMBER_OF_BANKS);
    }

    void onBankSettingChange() const override { display(); }

    uint8_t values[NUMBER_OF_BANKS] = {};
};

} // namespace Bankable