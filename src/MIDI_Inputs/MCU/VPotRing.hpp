#pragma once

#include <Banks/BankableMIDIInputAddressable.hpp>
#include <MIDI_Inputs/MIDIInputElementCC.hpp>

namespace MCU {

constexpr static uint8_t VPotRingAddress = 0x30;

// TODO: why can't I use the min/max macros on Teensy?
inline int8_t minimum(int8_t a, int8_t b) { return a > b ? b : a; }
inline int8_t maximum(int8_t a, int8_t b) { return a < b ? b : a; }

class VPotRing_Base : public MIDIInputElementCC {
  protected:
    VPotRing_Base(); // unused, only for virtual inheritance
  public:
    VPotRing_Base(uint8_t track, uint8_t channel = 1)
        : MIDIInputElementCC(channel, track + VPotRingAddress - 1) {}

    uint8_t getPosition() const { return getPosition(getValue()); }
    bool getCenterLed() const { return getCenterLed(getValue()); }
    uint8_t getMode() const { return getMode(getValue()); }

    uint8_t getStartOn() const {
        if (getPosition() == 0)
            return 0;
        int8_t value = (int8_t)getPosition() - (int8_t)1;
        switch (getMode()) {
            case 0: return value;
            case 1: return minimum(value, 5);
            case 2: return 0;
            case 3: return maximum(5 - value, 0);
        }
        return 0; // Shouldn't happen, just keeps the compiler happy.
    }

    uint8_t getStartOff() const {
        uint8_t value = getPosition();
        switch (getMode()) {
            case 0: return value;
            case 1: return maximum(value, 6);
            case 2: return value;
            case 3: return minimum(5 + value, 11);
        }
        return 0; // Shouldn't happen, just keeps the compiler happy.
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

class VPotRing : public virtual VPotRing_Base {
  public:
    VPotRing(uint8_t track, uint8_t channel = 1)
        : VPotRing_Base(track, channel) {}

    void reset() final override { value = 0; }

  private:
    bool updateImpl(const MIDI_message_matcher &midimsg) override {
        this->value = sanitizeValue(midimsg.data2);
        return true;
    }

    uint8_t getValue() const override { return value; }

    uint8_t value = 0;
};

// -------------------------------------------------------------------------- //

namespace Bankable {

template <uint8_t N>
class VPotRing : public virtual VPotRing_Base,
                 public BankableMIDIInputAddressable<N> {
  public:
    VPotRing(const BankConfigAddressable<N> &config, uint8_t track,
             uint8_t channel = 1)
        : VPotRing_Base(track, channel), BankableMIDIInputAddressable<N>(config) {}

    void reset() final override {
        for (uint8_t &value : values)
            value = 0;
    }

  private:
    bool updateImpl(const MIDI_message_matcher &midimsg) override {
        uint8_t index = getIndex(midimsg.channel, midimsg.data1,
                                 getBaseChannel(), getBaseAddress());
        uint8_t value = sanitizeValue(midimsg.data2);
        values[index] = value;
        return true;
    }

    bool matchAddress(uint8_t targetAddress) const override {
        return BankableMIDIInputAddressable<N>::matchAddress(
            targetAddress, getBaseAddress(), N); // TODO: N is known
    }

    uint8_t getValue() const override {
        return values[this->getSelection() % N]; // TODO: N
    }

    void onBankSettingChange() const override { display(); }

    uint8_t values[N] = {};
};

} // namespace Bankable

} // namespace MCU