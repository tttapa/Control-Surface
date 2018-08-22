#pragma once

#include <Banks/BankableMIDIInputAddressable.hpp>
#include <MIDI_Inputs/MIDIInputElementCC.hpp>

namespace MCU {

constexpr static uint8_t VPotRingAddress = 0x30;

inline int8_t minimum(int8_t a, int8_t b) { return a > b ? b : a; }
inline int8_t maximum(int8_t a, int8_t b) { return a < b ? b : a; }

/** 
 * @brief   An abstract class of MIDI input element that represent Mackie
 *          Control Universal V-Pots.  
 *          This is a base class to both the Bankable and non-Bankable version.
 */
class VPotRing_Base : public MIDIInputElementCC {
  protected:
    VPotRing_Base(); // unused, only for virtual inheritance
                     // keeps compiler happy, but gives a linker error if it is
                     // accidentally used.
    VPotRing_Base(uint8_t track, uint8_t channel = 1)
        : MIDIInputElementCC(channel, track + VPotRingAddress - 1) {}

  public:
    /** Return the position of the V-Pot ring. [0, 11] */
    uint8_t getPosition() const { return getPosition(getRawValue()); }
    /** Return the status of the center LED of the V-Pot ring. */
    bool getCenterLed() const { return getCenterLed(getRawValue()); }
    /** Return the mode of the V-Pot ring: 0 = single dot, 1 = boost/cut, 
     * 2 = wrap, 3 = spread */
    uint8_t getMode() const { return getMode(getRawValue()); }

    /** Get the first segment that should be on. */
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

    /** Get the first segment that should be off. */
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
    /** Make sure that the received value is valid and will not result in array
     * out of bounds conditions. */
    static inline uint8_t sanitizeValue(uint8_t value) {
        return (value & 0x0F) < 0x0C ? value : ((value & 0xF0) | 0xB);
    }

  private:
    /** Extract the position from the raw value. */
    static inline uint8_t getPosition(uint8_t value) { return value & 0x0F; }
    /** Extract the center LED state from the raw value. */
    static inline bool getCenterLed(uint8_t value) { return value & 0x40; }
    /** Extract the mode from the raw value. */
    static inline uint8_t getMode(uint8_t value) { return (value & 0x30) >> 4; }

    /** Return the raw value of the V-Pot. This contains the position, mode and
     * center LED state. */
    virtual uint8_t getRawValue() const = 0;
};

// -------------------------------------------------------------------------- //

/** 
 * @brief   A class for MIDI input elements that represent Mackie Control 
 *          Universal V-Pots.  
 *          This version cannot be banked.
 */
class VPotRing : public virtual VPotRing_Base {
  public:
    VPotRing(uint8_t track, uint8_t channel = 1)
        : VPotRing_Base(track, channel) {}

    void reset() override { value = 0; }

  private:
    bool updateImpl(const MIDI_message_matcher &midimsg) override {
        this->value = sanitizeValue(midimsg.data2);
        return true;
    }

    uint8_t getRawValue() const override { return value; }

    uint8_t value = 0;
};

// -------------------------------------------------------------------------- //

namespace Bankable {

/** 
 * @brief   A class for MIDI input elements that represent Mackie Control
 *          Universal V-Pots.  
 *          This version can be banked.
 * 
 * @tparam  N 
 *          The number of banks.
 */
template <uint8_t N>
class VPotRing : public virtual VPotRing_Base,
                 public BankableMIDIInputAddressable<N> {
  public:
    VPotRing(const BankConfigAddressable<N> &config, uint8_t track,
             uint8_t channel = 1)
        : VPotRing_Base(track, channel), BankableMIDIInputAddressable<N>(
                                             config) {}

    void reset() final override {
        for (uint8_t &value : values)
            value = 0;
    }

  private:
    bool updateImpl(const MIDI_message_matcher &midimsg) override {
        uint8_t index = this->getIndex(midimsg.channel, midimsg.data1,
                                       getBaseChannel(), getBaseAddress());
        uint8_t value = sanitizeValue(midimsg.data2);
        values[index] = value;
        return true;
    }

    bool matchAddress(uint8_t targetAddress) const override {
        return BankableMIDIInputAddressable<N>::matchAddress(targetAddress,
                                                             getBaseAddress());
    }

    uint8_t getRawValue() const override {
        return values[this->getSelection()]; // getSelection is guaranteed to be
                                             // in [0, N)
    }

    void onBankSettingChange() const override { display(); } // TODO: same in VU

    uint8_t values[N] = {};
};

} // namespace Bankable

} // namespace MCU