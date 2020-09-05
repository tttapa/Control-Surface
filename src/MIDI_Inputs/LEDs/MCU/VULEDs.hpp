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
    /// Constructor.
    ///
    /// @param  track
    ///         The track to listen for [1, 8].
    VULEDs(const PinList<NumLEDs> &leds, uint8_t track, MIDIChannelCN channel,
           unsigned int decayTime = VUDecay::Default)
        : VU(track, channel, decayTime), VULEDsDriver<NumLEDs>(leds) {}

    /// Constructor.
    ///
    /// @param  track
    ///         The track to listen for [1, 8].
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
    /// Constructor.
    ///
    /// @param  config
    ///         The bank configuration to use.
    /// @param  track
    ///         The track to listen for [1, 8].
    /// @param  channel
    ///         The MIDI channel and cable.
    VULEDs(BankConfig<BankSize> config, const PinList<NumLEDs> &leds,
           uint8_t track, MIDIChannelCN channel,
           unsigned int decayTime = VUDecay::Default)
        : VU<BankSize>(config, track, channel, decayTime),
          VULEDsDriver<NumLEDs>(leds) {}

    /// Constructor.
    ///
    /// @param  config
    ///         The bank configuration to use.
    /// @param  track
    ///         The track to listen for [1, 8].
    VULEDs(BankConfig<BankSize> config, const PinList<NumLEDs> &leds,
           uint8_t track, unsigned int decayTime = VUDecay::Default)
        : VU<BankSize>(config, track, decayTime), VULEDsDriver<NumLEDs>(leds) {}

  protected:
    void handleUpdate(typename VUMatcher<BankSize>::Result match) override {
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