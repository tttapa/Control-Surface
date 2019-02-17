#pragma once

#include <MIDI_Inputs/MIDIInputElementCC.hpp>

class MIDIInputCCValue : public MIDIInputElementCC {
  public:
    MIDIInputCCValue(const MIDICNChannelAddress &address)
        : MIDIInputElementCC(address) {}

    void reset() override { value = 0; }
    bool updateImpl(const MIDI_message_matcher &midimsg,
                    const MIDICNChannelAddress &target) override {
        value = midimsg.data2;
        return true;
    }

    uint8_t getValue() const { return value; }

  private:
    uint8_t value;
};

#include <MIDI_Inputs/DecayingVU.hpp>

class DecayingMIDIInputCCValue : public MIDIInputCCValue, public DecayingVU {
  public:
    DecayingMIDIInputCCValue(const MIDICNChannelAddress &address,
                             unsigned long decayTime)
        : MIDIInputCCValue(address), DecayingVU(decayTime) {}

  private:
    void update() override { DecayingVU::update(); }
};

class MIDIInputCCValuePWMLED : public MIDIInputCCValue {
  public:
    MIDIInputCCValuePWMLED(const MIDICNChannelAddress &address, pin_t ledPin)
        : MIDIInputCCValue(address), ledPin(ledPin) {}

  private:
    const pin_t ledPin;
    void display() const override {
        analogWrite(ledPin, getValue() << 2 | getValue() >> 6);
    }
};
