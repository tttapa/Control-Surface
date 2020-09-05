#pragma once

#include <AH/Hardware/LEDs/LEDs.hpp>
#include <MIDI_Inputs/MCU/VPotRing.hpp>

BEGIN_CS_NAMESPACE

namespace MCU {

class VPotRingLEDsDriver : public AH::LEDs<11> {
  public:
    VPotRingLEDsDriver(const AH::LEDs<11> &leds) : AH::LEDs<11>(leds) {}

    void displayVPot(VPotState v) {
        this->displayRange(v.getStartOn(), v.getStartOff());
    }
};

// -------------------------------------------------------------------------- //

/**
 * @brief   A MIDI input element that represents a Mackie Control Universal VPot
 *          ring and displays it using LEDs.
 * 
 * @ingroup midi-input-elements-leds
 */
class VPotRingLEDs : public VPotRing, public VPotRingLEDsDriver {
  public:
    /// Constructor.
    ///
    /// @param  track
    ///         The track to listen for [1, 8].
    VPotRingLEDs(const PinList<11> &leds, uint8_t track,
                 MIDIChannelCN channel = CHANNEL_1)
        : VPotRing(track, channel), VPotRingLEDsDriver(leds) {}

  protected:
    void handleUpdate(VPotMatcher::Result match) override {
        VPotRing::handleUpdate(match);
        updateDisplay();
    }

    /// If the state is dirty, update the LEDs
    void updateDisplay() {
        if (getDirty()) {
            this->displayVPot(this->getState());
            clearDirty();
        }
    }

  public:
    void begin() override {
        VPotRing::begin();
        VPotRingLEDsDriver::begin();
        updateDisplay();
    }

    void reset() override {
        VPotRing::reset();
        updateDisplay();
    }

  protected:
    using VPotRing::clearDirty;
    using VPotRing::getDirty;
};

namespace Bankable {

/**
 * @brief   A MIDI input element that represents a Mackie Control Universal VPot
 *          ring and displays its value using LEDs. This version can be banked.
 * 
 * @tparam  BankSize
 *          The number of banks.
 * 
 * @ingroup BankableMIDIInputElementsLEDs
 */
template <uint8_t BankSize>
class VPotRingLEDs : public VPotRing<BankSize>, public VPotRingLEDsDriver {
  public:
    /// Constructor.
    ///
    /// @param  track
    ///         The track to listen for [1, 8].
    VPotRingLEDs(BankConfig<BankSize> config, const PinList<11> &leds,
                 uint8_t track, MIDIChannelCN channel = CHANNEL_1)
        : VPotRing<BankSize>(config, track, channel), VPotRingLEDsDriver(leds) {
    }

  protected:
    void handleUpdate(typename VPotMatcher<BankSize>::Result match) override {
        VPotRing<BankSize>::handleUpdate(match);
        updateDisplay();
    }

    /// If the state is dirty, update the LEDs
    void updateDisplay() {
        if (getDirty()) {
            this->displayVPot(this->getState());
            clearDirty();
        }
    }

  public:
    void begin() override {
        VPotRing<BankSize>::begin();
        VPotRingLEDsDriver::begin();
        updateDisplay();
    }

    void reset() override {
        VPotRing<BankSize>::reset();
        updateDisplay();
    }

  protected:
    using VPotRing<BankSize>::clearDirty;
    using VPotRing<BankSize>::getDirty;

  protected:
    void onBankSettingChange() override {
        VPotRing<BankSize>::onBankSettingChange();
        updateDisplay();
    }
};

} // namespace Bankable

} // namespace MCU

END_CS_NAMESPACE