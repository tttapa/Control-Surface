#pragma once

#include <Banks/BankableMIDIInput.hpp>
#include <Hardware/ExtendedInputOutput/ExtendedInputOutput.h>
#include <Helpers/Copy.hpp>
#include <MIDI_Inputs/MIDIInputElementChannelPressure.hpp>
#include <string.h>

/** 
 * @brief   An abstract interface for VU meters. It declares two methods:
 *          `getValue` and `getOverload`.
 */
class IVU {
  public:
    /** Return the VU meter value as an integer. */
    virtual uint8_t getValue() const = 0;
    /** Return the overload status. */
    virtual bool getOverload() const = 0;
};

namespace MCU {

/** 
 * @brief   A MIDI input element that represents a Mackie Control Universal VU
 *          meter.  
 *          This is a base class to both the Bankable and non-Bankable version.
 */
class VU_Base : public MIDIInputElementChannelPressure, public IVU {
  public:
    VU_Base(); // Just for virtual inheritance
    VU_Base(const MIDICNChannelAddress &address, unsigned int decayTime)
        : MIDIInputElementChannelPressure{address - 1}, decayTime(decayTime) {}

    /** Return the VU meter value as an integer in [0, 12]. */
    uint8_t getValue() const override { return getValueHelper(getRawValue()); }
    /** Return the overload status. */
    bool getOverload() const override {
        return getOverloadHelper(getRawValue());
    }

  private:
    const unsigned long decayTime;
    unsigned long prevDecayTime = 0;

    void update() override {
        if (decayTime && ((millis() - prevDecayTime) >= decayTime)) {
            prevDecayTime += decayTime;
            decayAll();
            display();
        }
    }

    MIDICNChannelAddress
    getTarget(const MIDI_message_matcher &midimsg) const override {
        return {int8_t(midimsg.data1 >> 4), Channel(midimsg.channel)};
    }

    /** Automatically decay the VU meter value by one step (if it is greater
     * than zero). */
    virtual void decayAll() = 0;

    /** Return the raw value of the VU meter, this includes the actual VU value
     * and the overload status in a single byte. */
    virtual uint8_t getRawValue() const = 0;

  protected:
    /** Set the VU meter value.
     * @todo    rawValue reference instead of return.
     */
    uint8_t setValueHelper(uint8_t rawValue, uint8_t newValue) {
        prevDecayTime = millis();
        newValue |= rawValue & 0xF0;
        return newValue;
    }
    /** Set the overload status.
     * @todo    rawValue reference instead of return.
     */
    static inline uint8_t setOverloadHelper(uint8_t rawValue) {
        return rawValue |= 0xF0;
    }
    /** Clear the overload status.
     * @todo    rawValue reference instead of return.
     */
    static inline uint8_t clearOverloadHelper(uint8_t rawValue) {
        return rawValue &= 0x0F;
    }
    /** Get the VU meter value from the raw value. */
    static inline uint8_t getValueHelper(uint8_t rawValue) {
        return rawValue & 0x0F;
    }
    /** Get the overload status value from the raw value. */
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
     * @param   track
     *          The track of the VU meter. [1, 8]
     * @param   channel
     *          The MIDI channel. [1, 16]
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
    void decayAll() override {
        if (getValue() > 0)
            value--;
    }

    uint8_t getRawValue() const override { return value; }

    void setValue(uint8_t newValue) { value = setValueHelper(value, newValue); }
    void setOverload() { value = setOverloadHelper(value); }
    void clearOverload() { value = clearOverloadHelper(value); }

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
     * @param   track
     *          The track of the VU meter. [1, 8]
     * @param   channel
     *          The MIDI channel. [1, 16]
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
    void decayAll() override {
        for (uint8_t &value : values)
            if (getValueHelper(value) > 0)
                value--;
    }

    uint8_t getRawValue() const override {
        return values[this->getSelection()];
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

    uint8_t values[N] = {};
};

} // namespace Bankable

} // namespace MCU