#pragma once

#include <Banks/BankableMIDIInput.hpp>
#include <Hardware/ExtendedInputOutput/ExtendedInputOutput.hpp>
#include <Helpers/MinMaxFix.hpp>
#include <MIDI_Inputs/MIDIInputElementChannelPressure.hpp>
#include <string.h>

BEGIN_CS_NAMESPACE

/** 
 * @brief   An abstract interface for VU meters. To allow for both floating 
 *          point values and integers, all values are integers under the hood.
 * 
 * Using floats instead integers would be a strange choice as LED bar VU meters
 * have discrete levels.  
 * Continuous "analog" VU meters can use or override the `getFloatValue()` 
 * method.
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

/// VU Decay time constants
namespace VUDecay {
/// Don't decay automatically, hold the latest value until a new one is received.
constexpr unsigned int Hold = 0;
/// Decay one segment/block every 150 ms if no new values are received.
constexpr unsigned int Default = 150;
} // namespace VUDecay

/// Empty callback for VU meters that does nothing.
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
 * 
 * In the Mackie Control Universal protocol, VU meters are updated using Channel
 * Pressure events.  
 * Each device (cable number) has eight VU meters for the eight tracks. Only
 * MIDI channel 1 is used in the original protocol.
 * 
 * The format of the MIDI message is as follows:  
 * `| 1101 cccc | 0hhh llll |`
 * 
 * - `1101` (or `0xD`) is the status for Channel Pressure events
 * - `cccc` is the MIDI channel
 * - `hhh` is the track index [0-7]
 * - `llll` is the level of the VU meter
 * 
 * If the level is `0x0`, the meter is at 0%, if it's `0xC`, the meter is at 
 * 100%.  
 * `0xD` is an invalid value.  
 * `0xE` sets the overload indicator, and `0xF` clears the overload indicator.
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

    /// Update is called periodically, it decays the meter if the time is right.
    void update() override {
        if (decayTime && (millis() - prevDecayTime >= decayTime)) {
            prevDecayTime += decayTime;
            decay();
            callback.update(*this);
        }
    }

  private:
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

    /// The address of the VU meter is the high nibble of the first (and only)
    /// data byte.
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

    /// Get the active bank selection
    virtual uint8_t getSelection() const { return 0; }

    /// Get the bank index from a MIDI address
    virtual setting_t getBankIndex(const MIDICNChannelAddress &target) const {
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
 *          Universal VU meters. This version is generic to allow for custom 
 *          callbacks.  
 *          This version cannot be banked.
 */
template <class Callback = VUEmptyCallback>
class GenericVU : public VU_Base<1, Callback> {
  public:
    /** 
     * @brief   Construct a new GenericVU object.
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
     * @param   callback
     *          The callback object that is update when the value changes.
     *          Used for displaying the value on a range of LEDs etc.
     */
    GenericVU(uint8_t track, const MIDICNChannel &channelCN,
              unsigned int decayTime, const Callback &callback)
        : VU_Base<1, Callback>{
              track,
              channelCN,
              decayTime,
              callback,
          } {}
};

/** 
 * @brief   A class for MIDI input elements that represent Mackie Control
 *          Universal VU meters.  
 *          This version cannot be banked.
 * 
 * @ingroup MIDIInputElements
 */
class VU : public GenericVU<> {
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
    VU(uint8_t track, const MIDICNChannel &channelCN,
       unsigned int decayTime = VUDecay::Default)
        : GenericVU<>{
              track,
              channelCN,
              decayTime,
              {},
          } {}

    /** 
     * @brief   Construct a new VU object.
     * 
     * @param   track
     *          The track of the VU meter. [1, 8]
     * @param   decayTime
     *          The time in milliseconds it takes for the value to decay one
     *          step.  
     *          The MCU protocol uses 300 ms per division, and two steps
     *          per division, so the default is 150 ms per step.  
     *          Some software doesn't work if the VU meter decays automatically, 
     *          in that case, you can set the decay time to zero to disable 
     *          the decay.
     */
    VU(uint8_t track, unsigned int decayTime = VUDecay::Default)
        : GenericVU<>{
              track,
              CHANNEL_1,
              decayTime,
              {},
          } {}
};

// -------------------------------------------------------------------------- //

namespace Bankable {

/** 
 * @brief   A class for MIDI input elements that represent Mackie Control
 *          Universal VU meters.  This version is generic to allow for custom 
 *          callbacks.  
 *          This version can be banked.
 * 
 * @tparam  NumBanks
 *          The number of banks.
 */
template <uint8_t NumBanks, class Callback = VUEmptyCallback>
class GenericVU : public VU_Base<NumBanks, Callback>,
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
     * @param   callback
     *          The callback object that is update when the value or bank 
     *          changes.  
     *          Used for displaying the value on a range of LEDs etc.
     */
    GenericVU(const BankConfig<NumBanks> &config, uint8_t track,
              const MIDICNChannel &channelCN, unsigned int decayTime,
              const Callback &callback)
        : VU_Base<NumBanks, Callback>{
            track, 
            channelCN, 
            decayTime, 
            callback,
        },
        BankableMIDIInput<NumBanks>{config} {}

  private:
    setting_t getSelection() const override {
        return BankableMIDIInput<NumBanks>::getSelection();
    };

    uint8_t getBankIndex(const MIDICNChannelAddress &target) const override {
        return BankableMIDIInput<NumBanks>::getBankIndex(target, this->address);
    }

    /// Check if the address of the incoming MIDI message is in one of the banks
    /// of this element.
    bool match(const MIDICNChannelAddress &target) const override {
        return BankableMIDIInput<NumBanks>::matchBankable(target,
                                                          this->address);
    }

    void onBankSettingChange() override { this->callback.update(*this); }
};

/**
 * @brief   A class for MIDI input elements that represent Mackie Control
 *          Universal VU meters.  
 * 
 * @tparam  NumBanks 
 *          The number of banks.
 * 
 * @ingroup BankableMIDIInputElements
 */
template <uint8_t NumBanks>
class VU : public GenericVU<NumBanks> {
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
       const MIDICNChannel &channelCN,
       unsigned int decayTime = VUDecay::Default)
        : GenericVU<NumBanks>{
              config, track, channelCN, decayTime, {},
          } {}

    /** 
     * @brief   Construct a new Bankable VU object.
     * 
     * @param   config
     *          The bank configuration to use.
     * @param   track
     *          The track of the VU meter. [1, 8]
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
       unsigned int decayTime = VUDecay::Default)
        : GenericVU<NumBanks>{
              config, track, CHANNEL_1, decayTime, {},
          } {}
};

} // namespace Bankable

} // namespace MCU

END_CS_NAMESPACE