#pragma once

#include "MIDI_Input_Note.hpp"

class Note_Input_Base : public MIDI_Input_Note {
  protected:
    Note_Input_Base(); // unused, only for virtual inheritance
    Note_Input_Base(uint8_t track, uint8_t channel = 1)
        : MIDI_Input_Note(channel, track) {}

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

class Note_Input : public virtual Note_Input_Base {
  public:
    Note_Input(uint8_t track, uint8_t channel = 1)
        : Note_Input_Base(track, channel) {}

    uint8_t getValue() const override { return value; }

  private:
    bool updateImpl(const MIDI_message_matcher &midimsg) override {
        this->value = getValueFromMIDIMessage(midimsg);
        return true;
    }

    uint8_t value = 0;
};

// -------------------------------------------------------------------------- //

#include "../Banks/Bankable.hpp"

template <uint8_t NUMBER_OF_BANKS>
class Note_Input_Bankable : public virtual Note_Input_Base, public Bankable {
  public:
    Note_Input_Bankable(uint8_t track, uint8_t channel = 1)
        : Note_Input_Base(track, channel) {}

    uint8_t getValue() const override {
        return values[getBankSetting() % NUMBER_OF_BANKS];
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
        return Bankable::matchAddress(targetAddress, getBaseAddress(),
                                      NUMBER_OF_BANKS);
    }

    bool matchChannel(uint8_t targetChannel) const override {
        return Bankable::matchChannel(targetChannel, getBaseChannel(),
                                      NUMBER_OF_BANKS);
    }

    void onBankSettingChange() const override { display(); }

    uint8_t values[NUMBER_OF_BANKS] = {};
};