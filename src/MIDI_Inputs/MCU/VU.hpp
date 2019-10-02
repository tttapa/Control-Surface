#pragma once

#include <Banks/BankableMIDIInput.hpp>
#include <Hardware/ExtendedInputOutput/ExtendedInputOutput.hpp>
#include <Helpers/MinMaxFix.hpp>
#include <MIDI_Inputs/MIDIInputElementChannelPressure.hpp>
#include <string.h>

BEGIN_CS_NAMESPACE

/** 
 * @brief   An abstract interface for VU meters. It declares two methods:
 *          `getValue` and `getOverload`.
 */
class IVU {
  public:
    IVU(uint8_t max) : max(max) {}
    /** Return the VU meter value as an integer. */
    virtual uint8_t getValue() = 0;
    /** Return the overload status. */
    virtual bool getOverload() = 0;
    /** Get the VU meter value as a floating point number. */
    virtual float getFloatValue() { return (float)getValue() / getMax(); }
    /** Get the maximum value that this VU meter can return. */
    uint8_t getMax() const { return max; }

  protected:
    const uint8_t max;
};

namespace MCU {

struct VUEmptyCallback {
    template <class T>
    void begin(const T &) {}
    template <class T>
    void update(const T &) {}
};

/** 
 * @brief   A MIDI input element that represents a Mackie Control Universal VU
 *          meter.  
 *          This is a base class to both the Bankable and non-Bankable version.
 */
template <uint8_t NumValues, class Callback>
class VU_Base : public MIDIInputElementChannelPressure, public IVU {
  protected:
    VU_Base(uint8_t track, const MIDICNChannel &channelCN,
            unsigned int decayTime, const Callback &callback)
        : MIDIInputElementChannelPressure{{track - 1, channelCN}}, IVU(12),
          decayTime(decayTime), callback{callback} {}

  public:
    /// Initialize
    void begin() override { callback.begin(*this); }
    /// Reset all values to zero
    void reset() override {
        values = {};
        callback.update(*this);
    }

    /// Return the VU meter value as an integer in [0, 12].
    uint8_t getValue() override { return getValue(getSelection()); }
    /// Return the overload status.
    bool getOverload() override { return getOverload(getSelection()); }

    /// @todo   Move to an easily accessible namespace
    constexpr static unsigned int NO_DECAY = 0;

  private:
    /// Update is called periodically
    void update() override {
        if (decayTime && (millis() - prevDecayTime >= decayTime)) {
            prevDecayTime += decayTime;
            decay();
            callback.update(*this);
        }
    }

    /// Called when an incoming MIDI message matches this element
    bool updateImpl(const ChannelMessageMatcher &midimsg,
                    const MIDICNChannelAddress &target) override {
        uint8_t data = midimsg.data1 & 0x0F;
        uint8_t index = getBankIndex(target);
        switch (data) {
            case 0xF: clearOverload(index); break;
            case 0xE: setOverload(index); break;
            case 0xD: break; // no meaning
            default: setValue(index, data); break;
        }
        callback.update(*this);
        return true;
    }

    MIDICNChannelAddress
    getTarget(const ChannelMessageMatcher &midimsg) const override {
        return {
            int8_t(midimsg.data1 >> 4),
            Channel(midimsg.channel),
            midimsg.CN,
        };
    }

    void decay() {
        for (uint8_t i = 0; i < NumValues; ++i)
            if (getValue(i) > 0)
                values[i]--;
    }

    virtual uint8_t getSelection() const { return 0; }
    virtual uint8_t getBankIndex(const MIDICNChannelAddress &target) const {
        (void)target;
        return 0;
    }

    /// Set the VU meter value.
    void setValue(uint8_t index, uint8_t newValue) {
        prevDecayTime = millis();
        values[index] = newValue | (values[index] & 0xF0);
    }

    /// Set the overload status.
    void setOverload(uint8_t index) { values[index] |= 0xF0; }
    /// Clear the overload status.
    void clearOverload(uint8_t index) { values[index] &= 0x0F; }
    /// Get the VU meter value from the raw value.
    uint8_t getValue(uint8_t index) const { return values[index] & 0x0F; }
    /// Get the overload status value from the raw value.
    bool getOverload(uint8_t index) const { return values[index] & 0xF0; }

  private:
    Array<uint8_t, NumValues> values = {};
    unsigned int decayTime;
    unsigned long prevDecayTime = 0;

  public:
    Callback callback;
};

// -------------------------------------------------------------------------- //

/** 
 * @brief   A class for MIDI input elements that represent Mackie Control
 *          Universal VU meters.  
 *          This version cannot be banked.
 */
template <class Callback = VUEmptyCallback>
class VU_Generic : public VU_Base<1, Callback> {
  public:
    /** 
     * @brief   Construct a new VU object.
     * 
     * @param   track
     *          The track of the VU meter. [1, 8]
     * @param   channelCN
     *          The MIDI channel [CHANNEL_1, CHANNEL_16] and optional Cable
     *          Number [0, 15].
     * @param   decayTime
     *          The time in milliseconds it takes for the value to decay one
     *          step.  
     *          The MCU protocol uses 300 ms per division, and two steps
     *          per division, so the default is 150 ms per step.  
     *          Some software doesn't work if the VU meter decays automatically, 
     *          in that case, you can set the decay time to zero to disable 
     *          the decay.
     */
    VU_Generic(uint8_t track, const MIDICNChannel &channelCN,
               unsigned int decayTime = 150, const Callback &callback = {})
        : VU_Base<1, Callback>{track, channelCN, decayTime, callback} {}
};

using VU = VU_Generic<>;

// -------------------------------------------------------------------------- //

namespace Bankable {

/** 
 * @brief   A class for MIDI input elements that represent Mackie Control
 *          Universal VU meters.  
 *          This version can be banked.
 * 
 * @tparam  NumBanks
 *          The number of banks.
 */
template <uint8_t NumBanks, class Callback = VUEmptyCallback>
class VU_Generic : public VU_Base<NumBanks, Callback>,
                   public BankableMIDIInput<NumBanks> {
  public:
    /** 
     * @brief   Construct a new Bankable VU object.
     * 
     * @param   config
     *          The bank configuration to use.
     * @param   track
     *          The track of the VU meter. [1, 8]
     * @param   channelCN
     *          The MIDI channel [CHANNEL_1, CHANNEL_16] and optional Cable
     *          Number [0, 15].
     * @param   decayTime
     *          The time in milliseconds it takes for the value to decay one
     *          step.  
     *          The MCU protocol uses 300 ms per division, and two steps
     *          per division, so the default is 150 ms per step.  
     *          Some software doesn't work if the VU meter decays automatically, 
     *          in that case, you can set the decay time to zero to disable 
     *          the decay.
     */
    VU_Generic(const BankConfig<NumBanks> &config, uint8_t track,
               const MIDICNChannel &channelCN, unsigned int decayTime,
               const Callback &callback)
        : VU_Base<NumBanks, Callback>{track, channelCN, decayTime, callback},
          BankableMIDIInput<NumBanks>(config) {}

    using BankableMIDIInput<NumBanks>::getSelection;
    using BankableMIDIInput<NumBanks>::getBankIndex;

  protected:
    bool match(const MIDICNChannelAddress &target) const override {
        return BankableMIDIInput<NumBanks>::matchBankable(target,
                                                          this->address);
    }

    void onBankSettingChange() override { this->callback.update(*this); }
};

template <uint8_t NumBanks>
class VU : public VU_Generic<NumBanks> {
  public:
    /** 
     * @brief   Construct a new Bankable VU object.
     * 
     * @param   config
     *          The bank configuration to use.
     * @param   track
     *          The track of the VU meter. [1, 8]
     * @param   channelCN
     *          The MIDI channel [CHANNEL_1, CHANNEL_16] and optional Cable
     *          Number [0, 15].
     * @param   decayTime
     *          The time in milliseconds it takes for the value to decay one
     *          step.  
     *          The MCU protocol uses 300 ms per division, and two steps
     *          per division, so the default is 150 ms per step.  
     *          Some software doesn't work if the VU meter decays automatically, 
     *          in that case, you can set the decay time to zero to disable 
     *          the decay.
     */
    VU(const BankConfig<NumBanks> &config, uint8_t track,
       const MIDICNChannel &channelCN, unsigned int decayTime = 150)
        : VU_Generic<NumBanks>{config, track, channelCN, decayTime, {}} {}
};

} // namespace Bankable

} // namespace MCU

END_CS_NAMESPACE