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
    using Parent = VPotRing;
    using Matcher = Parent::Matcher;

    /** 
     * Constructor.
     * 
     * @param   leds
     *          The pins with LEDs connected.
     * @param   track
     *          The track of the VPot. [1, 8]
     * @param   channelCN
     *          The MIDI channel [Channel_1, Channel_16] and Cable
     *          Number [Cable_1, Cable_16].
     */
    VPotRingLEDs(const PinList<11> &leds, uint8_t track,
                 MIDIChannelCable channelCN = Channel_1)
        : Parent(track, channelCN), VPotRingLEDsDriver(leds) {}

  protected:
    void handleUpdate(typename Matcher::Result match) override {
        bool newdirty = Parent::handleUpdateImpl(match);
        if (newdirty)
            updateDisplay();
        this->dirty |= newdirty;
    }

    void updateDisplay() { this->displayVPot(this->getState()); }

  public:
    void begin() override {
        Parent::begin();
        VPotRingLEDsDriver::begin();
        updateDisplay();
    }

    void reset() override {
        Parent::reset();
        updateDisplay();
    }
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
    using Parent = VPotRing<BankSize>;
    using Matcher = typename Parent::Matcher;

    /** 
     * Constructor.
     * 
     * @param   config
     *          The bank configuration to use: the bank to add this element to,
     *          and whether to change the address, channel or cable number.
     * @param   leds
     *          The pins with LEDs connected.
     * @param   track
     *          The track of the VPot. [1, 8]
     * @param   channelCN
     *          The MIDI channel [Channel_1, Channel_16] and Cable
     *          Number [Cable_1, Cable_16].
     */
    VPotRingLEDs(BankConfig<BankSize> config, const PinList<11> &leds,
                 uint8_t track, MIDIChannelCable channelCN = Channel_1)
        : Parent(config, track, channelCN), VPotRingLEDsDriver(leds) {}

  protected:
    void handleUpdate(typename Matcher::Result match) override {
        bool newdirty = Parent::handleUpdateImpl(match);
        if (newdirty)
            updateDisplay();
        this->dirty |= newdirty;
    }

    void updateDisplay() { this->displayVPot(this->getState()); }

  public:
    void begin() override {
        Parent::begin();
        VPotRingLEDsDriver::begin();
        updateDisplay();
    }

    void reset() override {
        Parent::reset();
        updateDisplay();
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