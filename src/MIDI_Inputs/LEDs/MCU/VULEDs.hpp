#pragma once

#include <AH/Hardware/LEDs/DotBarDisplayLEDs.hpp>
#include <MIDI_Inputs/MCU/VU.hpp>

BEGIN_CS_NAMESPACE

namespace MCU {

/// Small helper to display the VU meters on an LED bar display.
template <uint8_t NumLEDs>
class VULEDsDriver : public AH::DotBarDisplayLEDs<NumLEDs> {
  public:
    /// Constructor
    VULEDsDriver(const PinList<NumLEDs> &leds)
        : AH::DotBarDisplayLEDs<NumLEDs>(leds) {}

    /// @param  value
    ///         [0, 12]
    void displayVU(uint8_t value) {
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
    /** 
     * Constructor.
     * 
     * @param   leds
     *          The pins with the LEDs connected.
     * @param   track
     *          The track of the VU meter. [1, 8]
     * @param   channelCN
     *          The MIDI channel [CHANNEL_1, CHANNEL_16] and Cable
     *          Number [CABLE_1, CABLE_16].
     * @param   decayTime
     *          The time in milliseconds it takes for the value to decay one
     *          step.  
     *          The MCU protocol uses 300 ms per division, and two steps
     *          per division, so the default is 150 ms per step.  
     *          Some software doesn't work if the VU meter decays automatically, 
     *          in that case, you can set the decay time to zero to disable 
     *          the decay.
     *          @see    @ref MCU::VUDecay
     */
    VULEDs(const PinList<NumLEDs> &leds, uint8_t track, MIDIChannelCN channelCN,
           unsigned int decayTime = VUDecay::Default)
        : VU(track, channelCN, decayTime), VULEDsDriver<NumLEDs>(leds) {}

    /** 
     * Constructor.
     * 
     * @param   leds
     *          The pins with the LEDs connected.
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
     *          @see    @ref MCU::VUDecay
     */
    VULEDs(const PinList<NumLEDs> &leds, uint8_t track,
           unsigned int decayTime = VUDecay::Default)
        : VU(track, decayTime), VULEDsDriver<NumLEDs>(leds) {}

  protected:
    void handleUpdate(VUMatcher::Result match) override {
        VU::handleUpdate(match);
        updateDisplay();
    }

    /// If the state is dirty, update the LEDs
    void updateDisplay() {
        if (getDirty()) {
            this->displayVU(getValue());
            clearDirty();
        }
    }

  public:
    void begin() override {
        VU::begin();
        VULEDsDriver<NumLEDs>::begin();
        updateDisplay();
    }

    void reset() override {
        VU::reset();
        updateDisplay();
    }

    void update() override {
        VU::update();
        updateDisplay();
    }

  protected:
    using VU::clearDirty;
    using VU::getDirty;
};

// -------------------------------------------------------------------------- //

namespace Bankable {

/** 
 * @brief   A MIDI input element that represents a Mackie Control Universal VU
 *          meter and displays its value using LEDs.
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
    /** 
     * Constructor.
     * 
     * @param   config
     *          The bank configuration to use.
     * @param   leds
     *          The pins with the LEDs connected.
     * @param   track
     *          The track of the VU meter. [1, 8]
     * @param   channelCN
     *          The MIDI channel [CHANNEL_1, CHANNEL_16] and Cable
     *          Number [CABLE_1, CABLE_16].
     * @param   decayTime
     *          The time in milliseconds it takes for the value to decay one
     *          step.  
     *          The MCU protocol uses 300 ms per division, and two steps
     *          per division, so the default is 150 ms per step.  
     *          Some software doesn't work if the VU meter decays automatically, 
     *          in that case, you can set the decay time to zero to disable 
     *          the decay.
     *          @see    @ref MCU::VUDecay
     */
    VULEDs(BankConfig<BankSize> config, const PinList<NumLEDs> &leds,
           uint8_t track, MIDIChannelCN channelCN,
           unsigned int decayTime = VUDecay::Default)
        : VU<BankSize>(config, track, channelCN, decayTime),
          VULEDsDriver<NumLEDs>(leds) {}

    /** 
     * Constructor.
     * 
     * @param   config
     *          The bank configuration to use.
     * @param   leds
     *          The pins with the LEDs connected.
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
     *          @see    @ref MCU::VUDecay
     */
    VULEDs(BankConfig<BankSize> config, const PinList<NumLEDs> &leds,
           uint8_t track, unsigned int decayTime = VUDecay::Default)
        : VU<BankSize>(config, track, decayTime), VULEDsDriver<NumLEDs>(leds) {}

  protected:
    void
    handleUpdate(typename BankableVUMatcher<BankSize>::Result match) override {
        VU<BankSize>::handleUpdate(match);
        updateDisplay();
    }

    /// If the state is dirty, update the LEDs
    void updateDisplay() {
        if (getDirty()) {
            this->displayVU(this->getValue());
            clearDirty();
        }
    }

  public:
    void begin() override {
        VU<BankSize>::begin();
        VULEDsDriver<NumLEDs>::begin();
        updateDisplay();
    }

    void reset() override {
        VU<BankSize>::reset();
        updateDisplay();
    }

    void update() override {
        VU<BankSize>::update();
        updateDisplay();
    }

  protected:
    using VU<BankSize>::clearDirty;
    using VU<BankSize>::getDirty;
};

} // namespace Bankable

} // namespace MCU

END_CS_NAMESPACE