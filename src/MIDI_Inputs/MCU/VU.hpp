#pragma once

#include <Banks/BankableMIDIInput.hpp>
#include <Hardware/ExtendedInputOutput/ExtendedInputOutput.hpp>
#include <MIDI_Inputs/DecayingVU.hpp>
#include <MIDI_Inputs/MIDIInputElementChannelPressure.hpp>
#include <string.h>

/** 
 * @brief   An abstract interface for VU meters. It declares two methods:
 *          `getValue` and `getOverload`.
 */
class IVU {
  public:
    IVU(uint8_t max) : max(max) {}
    /** Return the VU meter value as an integer. */
    virtual uint8_t getValue() const = 0;
    /** Return the overload status. */
    virtual bool getOverload() const = 0;
    /** Get the maximum value that this VU meter can return. */
    uint8_t getMax() const { return max; }

  protected:
    const uint8_t max;
};

namespace MCU {

/** 
 * @brief   A MIDI input element that represents a Mackie Control Universal VU
 *          meter.  
 *          This is a base class to both the Bankable and non-Bankable version.
 */
class VU_Base : public MIDIInputElementChannelPressure,
                public IVU,
                public DecayingVU {
  public:
    VU_Base(); // Just for virtual inheritance
    VU_Base(const MIDICNChannelAddress &address, unsigned int decayTime)
        : MIDIInputElementChannelPressure{address - 1}, // tracks are zero-based
          IVU(12), DecayingVU(decayTime) {}

    /// Return the VU meter value as an integer in [0, 12].
    uint8_t getValue() const override { return getValueHelper(getRawValue()); }
    /// Return the overload status.
    bool getOverload() const override {
        return getOverloadHelper(getRawValue());
    }

  private:
    void update() override { DecayingVU::update(); }

    MIDICNChannelAddress
    getTarget(const MIDI_message_matcher &midimsg) const override {
        return {
            // The track number is the high nibble of the first data byte
            int8_t(midimsg.data1 >> 4),
            Channel(midimsg.channel),
            midimsg.CN,
        };
    }

    /// Return the raw value of the VU meter, this includes the actual VU value
    /// and the overload status in a single byte.
    virtual uint8_t getRawValue() const = 0;

  protected:
    /// Set the VU meter value.
    inline void setValueHelper(uint8_t &rawValue, uint8_t newValue) {
        resetDecayTimer();
        newValue &= 0x0F;
        rawValue &= 0xF0;
        rawValue |= newValue;
    }
    /// Set the overload status.
    static inline void setOverloadHelper(uint8_t &rawValue) {
        rawValue |= 0xF0;
    }
    /// Clear the overload status.
    static inline void clearOverloadHelper(uint8_t &rawValue) {
        rawValue &= 0x0F;
    }
    /// Get the VU meter value from the raw value.
    static inline uint8_t getValueHelper(uint8_t rawValue) {
        return rawValue & 0x0F;
    }
    /// Get the overload status value from the raw value.
    static inline bool getOverloadHelper(uint8_t rawValue) {
        return rawValue & 0xF0;
    }
};

// -------------------------------------------------------------------------- //

/** 
 * @brief   A class for MIDI input elements that represent Mackie Control
 *          Universal VU meters.  
 *          This version cannot be banked.
 */
class VU : virtual public VU_Base {
  public:
    /** 
     * @brief   Construct a new VU object.
     * 
     * @param   address
     *          The track of the VU meter. [1, 8]  
     *          The MIDI channel. [1, 16]  
     *          The MIDI Cable Number. [0x0, 0xF]
     * @param   decayTime
     *          The time in milliseconds it takes for the value to decay one
     *          step.  
     *          The MCU protocol uses 300 ms per division, and two steps
     *          per division, so the default is 150 ms per step.  
     *          Some software doesn't work if the VU meter decays automatically, 
     *          in that case, you can set the decay time to zero to disable 
     *          the decay.
     */
    VU(const MIDICNChannelAddress &address, unsigned int decayTime = 150)
        : VU_Base(address, decayTime) {}

    bool updateImpl(const MIDI_message_matcher &midimsg,
                    UNUSED_PARAM const MIDICNChannelAddress &target) override {
        uint8_t data = midimsg.data1 & 0x0F;
        switch (data) {
            case 0xF: clearOverload(); break;
            case 0xE: setOverload(); break;
            case 0xD: break; // no meaning
            default: setValue(data); break;
        }
        return true;
    }

    void reset() override { value = 0; }

  private:
    void display() const override {}

    void decayAll() override {
        if (getValue() > 0)
            value--;
    }

    uint8_t getRawValue() const override { return value; }

    void setValue(uint8_t newValue) { setValueHelper(value, newValue); }
    void setOverload() { setOverloadHelper(value); }
    void clearOverload() { clearOverloadHelper(value); }

    uint8_t value = 0;
};

// -------------------------------------------------------------------------- //

namespace Bankable {

/** 
 * @brief   A class for MIDI input elements that represent Mackie Control
 *          Universal VU meters.  
 *          This version can be banked.
 * 
 * @tparam  N 
 *          The number of banks.
 */
template <size_t N>
class VU : virtual public VU_Base, public BankableMIDIInput<N> {
  public:
    /** 
     * @brief   Construct a new Bankable VU object.
     * 
     * @param   config
     *          The bank configuration to use.
     * @param   address
     *          The track of the VU meter. [1, 8]  
     *          The MIDI channel. [1, 16]  
     *          The MIDI Cable Number. [0x0, 0xF]
     * @param   decayTime
     *          The time in milliseconds it takes for the value to decay one
     *          step.  
     *          The MCU protocol uses 300 ms per division, and two steps
     *          per division, so the default is 150 ms per step.  
     *          Some software doesn't work if the VU meter decays automatically, 
     *          in that case, you can set the decay time to zero to disable 
     *          the decay.
     */
    VU(const BankConfig<N> &config, const MIDICNChannelAddress &address,
       unsigned int decayTime = 150)
        : VU_Base(address, decayTime), BankableMIDIInput<N>(config) {}

    bool updateImpl(const MIDI_message_matcher &midimsg,
                    const MIDICNChannelAddress &target) override {
        uint8_t index =
            this->getIndex(target, address) % N; // Todo: modulo everywhere?
        // TODO: I don't think getIndex cannot return any number >= N,
        // because otherwise, it wouldn't have matched, right?
        uint8_t data = midimsg.data1 & 0x0F;
        switch (data) {
            case 0xF: clearOverload(index); break;
            case 0xE: setOverload(index); break;
            case 0xD: break; // no meaning
            default: setValue(index, data); break;
        }
        return true;
    }

    bool match(const MIDICNChannelAddress &target) const override {
        return BankableMIDIInput<N>::matchBankable(target, address);
    }

    void reset() override {
        for (uint8_t &value : values)
            value = 0;
    }

  private:
    void display() const override {}

    void decayAll() override {
        for (uint8_t &value : values)
            if (getValueHelper(value) > 0)
                value--;
    }

    uint8_t getRawValue() const override {
        return values[this->getSelection()];
    }

    void setValue(uint8_t index, uint8_t newValue) {
        setValueHelper(values[index], newValue);
    }
    void setOverload(uint8_t index) { setOverloadHelper(values[index]); }
    void clearOverload(uint8_t index) { clearOverloadHelper(values[index]); }

    uint8_t values[N] = {};
};

} // namespace Bankable

} // namespace MCU