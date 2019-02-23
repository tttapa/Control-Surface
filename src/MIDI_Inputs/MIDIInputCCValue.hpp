#pragma once

#include <MIDI_Inputs/MIDIInputElementCC.hpp>

class MIDIInputCCValue : public MIDIInputElementCC {
  protected:
    MIDIInputCCValue(); // For virtual inheritance only

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

#include <Hardware/LEDs/VULEDs.hpp>

template <uint8_t N>
class MIDIInputCCValueLEDs_Base : virtual public MIDIInputCCValue {
  protected:
    MIDIInputCCValueLEDs_Base(const DotBarDisplayLEDs<N> &leds) : leds(leds) {}

  public:
    using MappingFunction = uint8_t (*)(uint8_t);

    void begin() override { leds.begin(); }

    static uint8_t defaultMappingFunction(uint8_t value) {
        return value * N / 127;
    }

    void map(MappingFunction fn) { this->fn = fn; }

  protected:
    void display() const override {
        uint8_t value = fn(this->getValue());
        if (value > N) {
            ERROR(F("Error: mapping function result is out of bounds (")
                      << +value << ')',
                  0x9823);
            value = N - 1;
        }
        leds.display(value);
    }

  private:
    const DotBarDisplayLEDs<N> leds;
    MappingFunction fn = defaultMappingFunction;
};

template <uint8_t N>
class MIDIInputCCValueLEDs : public MIDIInputCCValueLEDs_Base<N> {
  public:
    MIDIInputCCValueLEDs(const MIDICNChannelAddress &address,
                         const DotBarDisplayLEDs<N> &leds)
        : MIDIInputCCValue(address), MIDIInputCCValueLEDs_Base<N>(leds) {}

  protected:
    using MIDIInputCCValueLEDs_Base<N>::display;
    // void display() const override { MIDIInputCCValueLEDs_Base<N>::display(); }
};