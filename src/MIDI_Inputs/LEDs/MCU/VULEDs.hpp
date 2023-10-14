#pragma once

#include <AH/Hardware/LEDs/DotBarDisplayLEDs.hpp>
#include <MIDI_Inputs/MCU/VU.hpp>

BEGIN_CS_NAMESPACE

namespace MCU {

/// Small helper to display the VU meters on an LED bar display.
template <uint16_t NumLEDs>
class VULEDsDriver : public AH::DotBarDisplayLEDs<NumLEDs> {
  public:
    /// Constructor
    VULEDsDriver(const PinList<NumLEDs> &leds)
        : AH::DotBarDisplayLEDs<NumLEDs>(leds) {}

    /// @param  value
    ///         [0, 12]
    void displayVU(uint16_t value) {
        value = (value * NumLEDs + FLOOR_CORRECTION) / 12; // [0, N]
        this->display(value);
    }

  private:
    /// @see    doc/VU-LED-mapping.ods
    constexpr static uint8_t FLOOR_CORRECTION = 5;
};

// -------------------------------------------------------------------------- //

/** 
 * @brief   A MIDI input element that represents a Mackie Control Universal VU
 *          meter and displays its value using LEDs.
 * 
 * @tparam  NumLEDs
 *          The number of LEDs for the VU meter display. Recommended value is 
 *          12, because this is the range that's sent over MIDI.
 * 
 * @ingroup midi-input-elements-leds
 */
template <uint8_t NumLEDs>
class VULEDs : public VU, public VULEDsDriver<NumLEDs> {
  public:
    using Parent = VU;
    using Matcher = typename Parent::Matcher;

    /// @param  leds
    ///         The pins with the LEDs connected.
    /// @param  track
    ///         The track of the VU meter. [1, 8]
    /// @param  channelCN
    ///         The MIDI channel [Channel_1, Channel_16] and Cable
    ///         Number [Cable_1, Cable_16].
    /// @param  decayTime
    ///         The time in milliseconds it takes for the value to decay one
    ///         step.  
    ///         The MCU protocol uses 300 ms per division, and two steps
    ///         per division, so the default is 150 ms per step.  
    ///         Some software doesn't work if the VU meter decays automatically,
    ///         in that case, you can set the decay time to zero to disable
    ///         the decay.
    ///         @see    @ref MCU::VUDecay
    VULEDs(const PinList<NumLEDs> &leds, uint8_t track,
           MIDIChannelCable channelCN,
           unsigned int decayTime = VUDecay::Default)
        : Parent(track, channelCN, decayTime), VULEDsDriver<NumLEDs>(leds) {}

    /// @param  leds
    ///         The pins with the LEDs connected.
    /// @param  track
    ///         The track of the VU meter. [1, 8]
    /// @param  decayTime
    ///         The time in milliseconds it takes for the value to decay one
    ///         step.  
    ///         The MCU protocol uses 300 ms per division, and two steps
    ///         per division, so the default is 150 ms per step.  
    ///         Some software doesn't work if the VU meter decays automatically,
    ///         in that case, you can set the decay time to zero to disable
    ///         the decay.
    ///         @see    @ref MCU::VUDecay
    VULEDs(const PinList<NumLEDs> &leds, uint8_t track,
           unsigned int decayTime = VUDecay::Default)
        : Parent(track, decayTime), VULEDsDriver<NumLEDs>(leds) {}

  protected:
    void handleUpdate(typename Matcher::Result match) override {
        bool newdirty = Parent::handleUpdateImpl(match);
        if (newdirty)
            updateDisplay();
        this->dirty |= newdirty;
    }

    void updateDisplay() { this->displayVU(getValue()); }

  public:
    void begin() override {
        Parent::begin();
        VULEDsDriver<NumLEDs>::begin();
        updateDisplay();
    }

    void reset() override {
        Parent::reset();
        updateDisplay();
    }

    void update() override {
        bool newdirty = Parent::decay();
        if (newdirty)
            updateDisplay();
        this->dirty |= newdirty;
    }
};

// -------------------------------------------------------------------------- //

namespace Bankable {

/** 
 * @brief   A MIDI input element that represents a Mackie Control Universal VU
 *          meter and displays its value using LEDs.
 *          This version can be banked.
 * 
 * @tparam  BankSize
 *          The number of banks.
 * @tparam  NumLEDs
 *          The number of LEDs for the VU meter display. Recommended value is 
 *          12, because this is the range that's sent over MIDI.
 * 
 * @ingroup BankableMIDIInputElementsLEDs
 */
template <uint8_t BankSize, uint8_t NumLEDs>
class VULEDs : public VU<BankSize>, public VULEDsDriver<NumLEDs> {
  public:
    using Parent = VU<BankSize>;
    using Matcher = typename Parent::Matcher;

    /// @param  config
    ///         The bank configuration to use.
    /// @param  leds
    ///         The pins with the LEDs connected.
    /// @param  track
    ///         The track of the VU meter. [1, 8]
    /// @param  channelCN
    ///         The MIDI channel [Channel_1, Channel_16] and Cable
    ///         Number [Cable_1, Cable_16].
    /// @param  decayTime
    ///         The time in milliseconds it takes for the value to decay one
    ///         step.  
    ///         The MCU protocol uses 300 ms per division, and two steps
    ///         per division, so the default is 150 ms per step.  
    ///         Some software doesn't work if the VU meter decays automatically,
    ///         in that case, you can set the decay time to zero to disable
    ///         the decay.
    ///         @see    @ref MCU::VUDecay
    VULEDs(BankConfig<BankSize> config, const PinList<NumLEDs> &leds,
           uint8_t track, MIDIChannelCable channelCN,
           unsigned int decayTime = VUDecay::Default)
        : Parent(config, track, channelCN, decayTime),
          VULEDsDriver<NumLEDs>(leds) {}

    /// @param  config
    ///         The bank configuration to use.
    /// @param  leds
    ///         The pins with the LEDs connected.
    /// @param  track
    ///         The track of the VU meter. [1, 8]
    /// @param  decayTime
    ///         The time in milliseconds it takes for the value to decay one
    ///         step.  
    ///         The MCU protocol uses 300 ms per division, and two steps
    ///         per division, so the default is 150 ms per step.  
    ///         Some software doesn't work if the VU meter decays automatically,
    ///         in that case, you can set the decay time to zero to disable
    ///         the decay.
    ///         @see    @ref MCU::VUDecay
    VULEDs(BankConfig<BankSize> config, const PinList<NumLEDs> &leds,
           uint8_t track, unsigned int decayTime = VUDecay::Default)
        : Parent(config, track, decayTime), VULEDsDriver<NumLEDs>(leds) {}

  protected:
    void handleUpdate(typename Matcher::Result match) override {
        bool newdirty = Parent::handleUpdateImpl(match);
        if (newdirty)
            updateDisplay();
        this->dirty |= newdirty;
    }

    void updateDisplay() { this->displayVU(this->getValue()); }

  public:
    void begin() override {
        Parent::begin();
        VULEDsDriver<NumLEDs>::begin();
        updateDisplay();
    }

    void reset() override {
        Parent::reset();
        updateDisplay();
    }

    void update() override {
        bool newdirty = Parent::decay();
        if (newdirty)
            updateDisplay();
        this->dirty |= newdirty;
    }

  protected:
    void onBankSettingChange() override {
        Parent::onBankSettingChange();
        updateDisplay();
    }
};

} // namespace Bankable

} // namespace MCU

END_CS_NAMESPACE