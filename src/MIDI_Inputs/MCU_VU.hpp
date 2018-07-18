#pragma once

#include "MIDI_Input_ChannelPressure.hpp"
#include <Hardware/ExtendedInputOutput/ExtendedInputOutput.h>
#include <Helpers/Copy.hpp>
#include <string.h>

using namespace ExtIO;

// -------------------------------------------------------------------------- //

class MCU_VU_Base : public MIDI_Input_ChannelPressure {
  public:
    MCU_VU_Base(uint8_t track, uint8_t channel, unsigned int decayTime)
        : MIDI_Input_ChannelPressure(channel), baseTrack(track - 1),
          decayTime(decayTime) {}

    uint8_t getValue() const { return getValueHelper(getRawValue()); }
    bool getOverload() const { return getOverloadHelper(getRawValue()); }

  private:
    const uint8_t baseTrack;
    const unsigned long decayTime;
    unsigned long prevDecayTime = 0;

    void update() override {
        if (decayTime && ((millis() - prevDecayTime) >= decayTime)) {
            prevDecayTime += decayTime;
            decayAll();
            display();
        }
    }

    virtual void decayAll() = 0;

    virtual uint8_t getRawValue() const = 0;

  protected:
    uint8_t getBaseTrack() const { return baseTrack; }

    uint8_t setValueHelper(uint8_t rawValue, uint8_t newValue) {
        prevDecayTime = millis();
        newValue |= rawValue & 0xF0;
        return newValue;
    }
    static inline uint8_t setOverloadHelper(uint8_t rawValue) {
        return rawValue |= 0xF0;
    }
    static inline uint8_t clearOverloadHelper(uint8_t rawValue) {
        return rawValue &= 0x0F;
    }
    static inline uint8_t getValueHelper(uint8_t rawValue) {
        return rawValue & 0x0F;
    }
    static inline bool getOverloadHelper(uint8_t rawValue) {
        return rawValue & 0xF0;
    }
};

// -------------------------------------------------------------------------- //

class MCU_VU : virtual public MCU_VU_Base {
  public:
    MCU_VU(uint8_t track, uint8_t channel = 1, unsigned int decayTime = 300)
        : MCU_VU_Base(track, channel, decayTime) {}

    bool updateImpl(const MIDI_message_matcher &midimsg) {
        uint8_t targetTrack = midimsg.data1 >> 4;
        DEBUGFN("target track = " << +targetTrack);
        if (!matchTrack(targetTrack))
            return false;

        uint8_t data = midimsg.data1 & 0x0F;
        switch (data) {
            case 0xF: clearOverload(); break;
            case 0xE: setOverload(); break;
            case 0xD: break; // no meaning
            default: setValue(data); break;
        }
        display();
        return true;
    }

    void reset() override { value = 0; }

  private:
    void decayAll() override {
        if (getValue() > 0)
            value--;
    }

    uint8_t getRawValue() const override { return value; }

    inline bool matchTrack(uint8_t targetTrack) const {
        return targetTrack == getBaseTrack();
    }
    void setValue(uint8_t newValue) { value = setValueHelper(value, newValue); }
    void setOverload() { value = setOverloadHelper(value); }
    void clearOverload() { value = clearOverloadHelper(value); }

    uint8_t value = 0;
};

// -------------------------------------------------------------------------- //

#include <Banks/Bankable.hpp>

template <size_t NUMBER_OF_BANKS>
class MCU_VU_Bankable : virtual public MCU_VU_Base, public Bankable {
  public:
    MCU_VU_Bankable(uint8_t track, uint8_t channel = 1,
                    unsigned int decayTime = 300)
        : MCU_VU_Base(track, channel, decayTime) {}

    bool updateImpl(const MIDI_message_matcher &midimsg) {
        uint8_t targetTrack = midimsg.data1 >> 4;
        DEBUGFN("target track = " << +targetTrack);
        if (!matchTrack(targetTrack))
            return false;
        uint8_t index = getIndex(midimsg.channel, targetTrack, getBaseChannel(),
                                 getBaseTrack()) %
                        NUMBER_OF_BANKS;
        uint8_t data = midimsg.data1 & 0x0F;
        switch (data) {
            case 0xF: clearOverload(index); break;
            case 0xE: setOverload(index); break;
            case 0xD: break; // no meaning
            default: setValue(index, data); break;
        }
        display();
        return true;
    }

    void reset() override {
        for (uint8_t &value : values)
            value = 0;
    }

  private:
    void decayAll() override {
        for (uint8_t &value : values)
            if (getValueHelper(value) > 0)
                value--;
    }

    uint8_t getRawValue() const override {
        return values[getBankSetting() % NUMBER_OF_BANKS];
    }

    inline bool matchTrack(uint8_t targetTrack) const {
        return Bankable::matchAddress(targetTrack, getBaseTrack(),
                                      NUMBER_OF_BANKS);
    }
    inline bool matchChannel(uint8_t targetChannel) const override {
        return Bankable::matchChannel(targetChannel, getBaseChannel(),
                                      NUMBER_OF_BANKS);
    }
    void setValue(uint8_t index, uint8_t newValue) {
        values[index] = setValueHelper(values[index], newValue);
    }
    void setOverload(uint8_t index) {
        values[index] = setOverloadHelper(values[index]);
    }
    void clearOverload(uint8_t index) {
        values[index] = clearOverloadHelper(values[index]);
    }

    uint8_t values[NUMBER_OF_BANKS] = {};
};

// -------------------------------------------------------------------------- //
/*
class MCU_VU_LED : public MCU_VU {
  public:
    template <size_t N>
    MCU_VU_LED(const pin_t (&LEDs)[N], uint8_t address, uint8_t nb_addresses,
               bool decay = true)
        : MCU_VU(address, nb_addresses, decay), overloadpin(0), length(N),
          overload(false) {
        static_assert(
            N <= 12,
            "Error: the maximum number of LEDs in the VU meter is 12. ");
        this->LEDs = new pin_t[length];
        copy(this->LEDs, LEDs);
        for (pin_t pin = 0; pin < length; pin++)
            ExtIO::pinMode(LEDs[pin], OUTPUT);
    }
    ~MCU_VU_LED() { delete[] LEDs; }

  protected:
    const uint8_t length;
    const pin_t overloadpin;
    pin_t *LEDs;
    const bool overload;

    constexpr static uint8_t floorCorrection = 5;

    void display() {
        for (uint8_t pin = 0;
             pin < (getValue(addressOffset) * length + floorCorrection) / 12;
             pin++)
            digitalWrite(LEDs[pin], HIGH);
        for (uint8_t pin =
                 (getValue(addressOffset) * length + floorCorrection) / 12;
             pin < length; pin++)
            digitalWrite(LEDs[pin], LOW);
        if (overload)
            digitalWrite(overloadpin, (uint8_t)getOverload(addressOffset));
    }
};
*/