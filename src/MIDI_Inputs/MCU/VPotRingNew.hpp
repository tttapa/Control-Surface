#pragma once

#include <MIDI_Inputs/MIDIInputElementCC.hpp>
#include <MIDI_Inputs/MIDINoteCCRange.hpp>

BEGIN_CS_NAMESPACE

namespace MCU {

constexpr static uint8_t VPotRingAddress = 0x30;

inline int8_t minimum(int8_t a, int8_t b) { return a > b ? b : a; }
inline int8_t maximum(int8_t a, int8_t b) { return a < b ? b : a; }

template <class Parent, class Callback = MIDINoteCCRangeEmptyCallback>
class VPotRing_Base : public Parent {
  public:
    VPotRing_Base(const MIDICNChannelAddress &address, const Callback &callback)
        : Parent{address + VPotRingAddress, callback} {}

  public:
    /// Return the position of the V-Pot ring. [0, 11]
    uint8_t getPosition() const { return getPosition(getValue()); }
    /// Return the status of the center LED of the V-Pot ring.
    bool getCenterLed() const { return getCenterLed(getValue()); }
    /// Return the mode of the V-Pot ring: 0 = single dot, 1 = boost/cut,
    /// 2 = wrap, 3 = spread
    uint8_t getMode() const { return getMode(getValue()); }

    /// Get the first segment that should be on.
    uint8_t getStartOn() const {
        if (getPosition() == 0)
            return 0;
        int8_t value = (int8_t)getPosition() - (int8_t)1;
        switch (getMode()) {
            case 0: return value;
            case 1: return minimum(value, 5);
            case 2: return 0;
            case 3: return maximum(5 - value, 0);
            // Shouldn't happen, just keeps the compiler happy:
            default: return 0;
        }
    }

    /// Get the first segment that should be off.
    uint8_t getStartOff() const {
        uint8_t value = getPosition();
        switch (getMode()) {
            case 0: return value;
            case 1: return maximum(value, 6);
            case 2: return value;
            case 3: return minimum(5 + value, 11);
            // Shouldn't happen, just keeps the compiler happy:
            default: return 0;
        }
    }

  private:
    /// Extract the position from the raw value.
    static uint8_t getPosition(uint8_t value) {
        uint8_t position = value & 0x0F;
        return position < 0x0C ? position : 0x0B;
    }
    /// Extract the center LED state from the raw value.
    static bool getCenterLed(uint8_t value) { return value & 0x40; }
    /// Extract the mode from the raw value.
    static uint8_t getMode(uint8_t value) { return (value & 0x30) >> 4; }
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
class VPotRing : virtual public VPotRing_Base, public BankableMIDIInput<N> {
  public:
    VPotRing(const BankConfig<N> &config, const MIDICNChannelAddress &address)
        : VPotRing_Base(address), BankableMIDIInput<N>(config) {}

    void reset() final override {
        for (uint8_t &value : values)
            value = 0;
    }

  private:
    bool updateImpl(const ChannelMessageMatcher &midimsg,
                    const MIDICNChannelAddress &target) override {
        uint8_t index = this->getBankIndex(target, address);
        uint8_t value = sanitizeValue(midimsg.data2);
        values[index] = value;
        return true;
    }

    bool match(const MIDICNChannelAddress &target) const override {
        return BankableMIDIInput<N>::matchBankable(target, address);
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

END_CS_NAMESPACE