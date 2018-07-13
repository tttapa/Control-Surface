#pragma once

#include "MIDI_Input_CC.hpp"

constexpr static uint8_t VPotRingAddress = 0x30;

// TODO: why can't I use the min/max macros on Teensy?
inline int8_t minimum(int8_t a, int8_t b) { return a > b ? b : a; }
inline int8_t maximum(int8_t a, int8_t b) { return a < b ? b : a; }

class MCU_VPotRing_Base : public MIDI_Input_CC {
  protected:
    MCU_VPotRing_Base(); // unused, only for virtual inheritance
  public:
    MCU_VPotRing_Base(uint8_t track, uint8_t channel = 1)
        : MIDI_Input_CC(channel, track + VPotRingAddress - 1) {}

    uint8_t getPosition() const { return getPosition(getValue()); }
    bool getCenterLed() const { return getCenterLed(getValue()); }
    uint8_t getMode() const { return getMode(getValue()); }

    uint8_t getStartOn() const {
        int8_t value = (int8_t)getPosition() - (int8_t)1;
        switch (getMode()) {
        case 0:
            return value;
        case 1:
            return minimum(value, 5);
        case 2:
            return 0;
        case 3:
            return maximum(5 - value, 0);
        }
    }

    uint8_t getStartOff() const {
        int8_t value = (int8_t)getPosition() - (int8_t)1;
        switch (getMode()) {
        case 0:
            return value + 1;
        case 1:
            return maximum(value, 5) + 1;
        case 2:
            return value + 1;
        case 3:
            return minimum(6 + value, 11);
        }
    }

  protected:
    static inline uint8_t sanitizeValue(uint8_t value) {
        return (value & 0x0F) < 0x0C ? value : ((value & 0xF0) | 0xB);
    }

  private:
    virtual bool updateImpl(const MIDI_message_matcher &midimsg) = 0;

    static inline uint8_t getPosition(uint8_t value) { return value & 0x0F; }
    static inline bool getCenterLed(uint8_t value) { return value & 0x40; }
    static inline uint8_t getMode(uint8_t value) { return (value & 0x30) >> 4; }

    virtual uint8_t getValue() const = 0;
};

// -------------------------------------------------------------------------- //

class MCU_VPotRing : public virtual MCU_VPotRing_Base {
  public:
    MCU_VPotRing(uint8_t track, uint8_t channel = 1)
        : MCU_VPotRing_Base(track, channel) {}

  private:
    bool updateImpl(const MIDI_message_matcher &midimsg) override {
        this->value = sanitizeValue(midimsg.data2);
        return true;
    }

    uint8_t getValue() const override { return value; }

    uint8_t value = 0;
};

// -------------------------------------------------------------------------- //

#include "../Banks/Bankable.hpp"

template <uint8_t NUMBER_OF_BANKS>
class MCU_VPotRing_Bankable : public virtual MCU_VPotRing_Base,
                              public Bankable {
  public:
    MCU_VPotRing_Bankable(uint8_t track, uint8_t channel = 1)
        : MCU_VPotRing_Base(track, channel) {}

  private:
    bool updateImpl(const MIDI_message_matcher &midimsg) override {
        uint8_t index = getAddressIndex(midimsg.data1, getBaseAddress());
        uint8_t value = sanitizeValue(midimsg.data2);
        values[index] = value;
        return true;
    }

    bool matchAddress(uint8_t targetAddress) const override {
        return Bankable::matchAddress(targetAddress, getBaseAddress(),
                                      NUMBER_OF_BANKS);
    }

    uint8_t getValue() const override {
        return values[getBankSetting() % NUMBER_OF_BANKS];
    }

    uint8_t values[NUMBER_OF_BANKS] = {};
};