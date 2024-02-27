#pragma once

#include <AH/STL/algorithm>
#include <AH/Timing/MillisMicrosTimer.hpp>
#include <MIDI_Inputs/InterfaceMIDIInputElements.hpp>
#include <MIDI_Inputs/MIDIInputElementMatchers.hpp>

BEGIN_CS_NAMESPACE

namespace MCU {

/// Struct that keeps track of the value and overload indicator of a Mackie
/// Control Universal VPot LED ring.
struct VPotState {
    /// Constructor.
    VPotState(uint8_t value = 0) : value(value) {}

    uint8_t value; ///< The value representing the VPot position, mode and LED.

    bool update(uint8_t data) {
        data = sanitizeData(data);
        bool changed = data != this->value;
        this->value = data;
        return changed;
    }

    /// Determines how the VPot value is displayed using the LEDs.
    enum Mode {
        SingleDot = 0, ///< Single dot.
        BoostCut = 1,  ///< Boost/Cut.
        Wrap = 2,      ///< Wrap.
        Spread = 3,    ///< Spread.
    };

    /// @name Data access
    /// @{

    /// Return the position of the V-Pot ring. [0, 11]
    uint8_t getPosition() const { return value & 0x0F; }
    /// Return the status of the center LED of the V-Pot ring.
    bool getCenterLed() const { return value & 0x40; }
    /// Return the mode of the V-Pot ring.
    Mode getMode() const { return static_cast<Mode>((value & 0x30) >> 4); }

    /// Get the first segment that should be on.
    uint8_t getStartOn() const {
        int8_t position = getPosition();
        if (position == 0)
            return 0;
        int8_t value = position - 1;
        switch (getMode()) {
            case SingleDot: return value;
            case BoostCut: return std::min(+value, 5);
            case Wrap: return 0;
            case Spread: return std::max(5 - value, 0);
            default: return 0;
        }
    }

    /// Get the first segment that should be off.
    uint8_t getStartOff() const {
        uint8_t value = getPosition();
        switch (getMode()) {
            case SingleDot: return value;
            case BoostCut: return std::max(+value, 6);
            case Wrap: return value;
            case Spread: return std::min(5 + value, 11);
            default: return 0;
        }
    }

    /// @}

    /// Make sure that the received value is valid and will not result in array
    /// out of bounds conditions.
    static uint8_t sanitizeData(uint8_t data) {
        // Maximum valid position value is 0xB.
        return (data & 0x0F) <= 0x0B ? data : ((data & 0xF0) | 0x0B);
    }
};

// -------------------------------------------------------------------------- //

/**
 * @brief   MIDI Input matcher for Mackie Control Universal VPot LED rings.
 *
 * In the Mackie Control Universal protocol, the VPot LED rings are updated 
 * using Control Change events.  
 * Each device (cable number) has eight VPots for the eight tracks. Only
 * MIDI channel 1 is used in the original protocol.
 * 
 * The format of the MIDI message is as follows:
 * | Status      | Data 1      | Data 2      |
 * |:-----------:|:-----------:|:-----------:|
 * | `1011 cccc` | `0011 0ttt` | `0pxx vvvv` |
 * 
 * - `1011` (or `0xB`) is the status for Control Change events
 * - `cccc` is the MIDI channel [0-15]
 * - `ttt` is the track index [0-7]
 * - `p` is the state of the center LED
 * - `xx` is the display mode
 * - `vvvv` is the VPot value [0, 11]
 * 
 * The display modes are defined in @ref MCU::VPotState::Mode.
 * 
 * @ingroup    MIDIInputMatchers
 */
struct VPotMatcher : public TwoByteMIDIMatcher {
    /**
     * @brief   Constructor.
     * 
     * @param   track
     *          The track of the VPot [1, 8].
     * @param   channelCN
     *          The MIDI channel [Channel_1, Channel_16] and Cable Number 
     *          [Cable_1, Cable_16].
     */
    VPotMatcher(uint8_t track, MIDIChannelCable channelCN)
        : TwoByteMIDIMatcher({track + 0x30 - 1, channelCN}) {}
};

/// MIDI Input matcher for Mackie Control Universal VPot LED rings with bank
/// support.
/// @see    @ref MCU::VPotMatcher
/// @ingroup    MIDIInputMatchers
template <uint8_t BankSize>
struct BankableVPotMatcher : public BankableTwoByteMIDIMatcher<BankSize> {
    /**
     * @brief   Constructor.
     * 
     * @param   config
     *          The bank configuration to use: the bank to add this element to,
     *          and whether to change the address, channel or cable number.
     * @param   track
     *          The track of the VPot [1, 8].
     * @param   channelCN
     *          The MIDI channel [Channel_1, Channel_16] and Cable Number 
     *          [Cable_1, Cable_16].
     */
    BankableVPotMatcher(BankConfig<BankSize> config, uint8_t track,
                        MIDIChannelCable channelCN)
        : BankableTwoByteMIDIMatcher<BankSize>(config,
                                               {track + 0x30 - 1, channelCN}) {}
};

// -------------------------------------------------------------------------- //

/** 
 * @brief   A MIDI input element that represents a Mackie Control Universal VPot
 *          ring.
 * 
 * @ingroup MIDIInputElements
 */
class VPotRing : public MatchingMIDIInputElement<MIDIMessageType::ControlChange,
                                                 VPotMatcher>,
                 public Interfaces::MCU::IVPot {
  public:
    using Matcher = VPotMatcher;
    using Parent =
        MatchingMIDIInputElement<MIDIMessageType::ControlChange, Matcher>;

    /**
     * @brief   Constructor.
     * 
     * @param   track
     *          The track of the VPot [1, 8].
     * @param   channelCN
     *          The MIDI channel [Channel_1, Channel_16] and Cable Number 
     *          [Cable_1, Cable_16].
     */
    VPotRing(uint8_t track, MIDIChannelCable channelCN = Channel_1)
        : Parent({track, channelCN}) {}

  protected:
    bool handleUpdateImpl(typename Matcher::Result match) {
        return state.update(match.value);
    }

    void handleUpdate(typename Matcher::Result match) override {
        dirty |= handleUpdateImpl(match);
    }

  public:
    /// @name Data access
    /// @{

    /// Get the full state of the VPot ring.
    VPotState getState() const { return state; }

    /// @copydoc    VPotState::getPosition
    uint8_t getPosition() const { return state.getPosition(); }
    /// @copydoc    VPotState::getCenterLed
    bool getCenterLed() const override { return state.getCenterLed(); }
    /// @copydoc    VPotState::getMode
    VPotState::Mode getMode() const { return state.getMode(); }

    /// @copydoc    VPotState::getStartOn
    uint8_t getStartOn() const override { return state.getStartOn(); }
    /// @copydoc    VPotState::getStartOff
    uint8_t getStartOff() const override { return state.getStartOff(); }

    /// @}

    /// Reset the state to zero.
    void reset() override {
        if (!ignoreReset) {
            state = {};
            dirty = true;
        }
    }

  private:
    VPotState state = {};

  public:
    /// Don't reset the state when calling the `reset` method. This is the
    /// default, because in the original MCU, VPots don't get reset when a
    /// Reset All Controllers message is received.
    bool ignoreReset = true;
};

// -------------------------------------------------------------------------- //

namespace Bankable {

/** 
 * @brief   A MIDI input element that represents a Mackie Control Universal VPot
 *          ring. This version can be banked.
 *
 * @tparam  BankSize
 *          The number of banks.
 * 
 * @ingroup BankableMIDIInputElements
 */
template <uint8_t BankSize>
class VPotRing
    : public BankableMatchingMIDIInputElement<MIDIMessageType::ControlChange,
                                              BankableVPotMatcher<BankSize>>,
      public Interfaces::MCU::IVPot {
  public:
    using Matcher = BankableVPotMatcher<BankSize>;
    using Parent =
        BankableMatchingMIDIInputElement<MIDIMessageType::ControlChange,
                                         Matcher>;
    /**
     * @brief   Constructor.
     * 
     * @param   config
     *          The bank configuration to use: the bank to add this element to,
     *          and whether to change the address, channel or cable number.
     * @param   track
     *          The track of the VPot [1, 8].
     * @param   channelCN
     *          The MIDI channel [Channel_1, Channel_16] and Cable Number 
     *          [Cable_1, Cable_16].
     */
    VPotRing(BankConfig<BankSize> config, uint8_t track,
             MIDIChannelCable channelCN = Channel_1)
        : Parent({config, track, channelCN}) {}

  protected:
    bool handleUpdateImpl(typename Matcher::Result match) {
        return states[match.bankIndex].update(match.value) &&
               match.bankIndex == this->getActiveBank();
        // Only mark dirty if the value of the active bank changed
    }

    void handleUpdate(typename Matcher::Result match) override {
        dirty |= handleUpdateImpl(match);
    }

  public:
    /// @name Data access
    /// @{

    /// Get the full state of the VPot ring. (For the given bank.)
    VPotState getState(uint8_t bank) const { return states[bank]; }

    /// @copydoc    VPotState::getPosition
    /// (For the given bank.)
    uint8_t getPosition(uint8_t bank) const {
        return getState(bank).getPosition();
    }
    /// @copydoc    VPotState::getCenterLed
    /// (For the given bank.)
    bool getCenterLed(uint8_t bank) const {
        return getState(bank).getCenterLed();
    }
    /// @copydoc    VPotState::getMode
    /// (For the given bank.)
    VPotState::Mode getMode(uint8_t bank) const {
        return getState(bank).getMode();
    }

    /// @copydoc    VPotState::getStartOn
    /// (For the given bank.)
    uint8_t getStartOn(uint8_t bank) const {
        return getState(bank).getStartOn();
    }
    /// @copydoc    VPotState::getStartOff
    /// (For the given bank.)
    uint8_t getStartOff(uint8_t bank) const {
        return getState(bank).getStartOff();
    }

    /// Get the full state of the VPot ring. (For the active bank.)
    VPotState getState() const { return getState(this->getActiveBank()); }

    /// @copydoc    VPotState::getPosition
    /// (For the active bank.)
    uint8_t getPosition() const { return getPosition(this->getActiveBank()); }
    /// @copydoc    VPotState::getCenterLed
    /// (For the active bank.)
    bool getCenterLed() const override {
        return getCenterLed(this->getActiveBank());
    }
    /// @copydoc    VPotState::getMode
    /// (For the active bank.)
    VPotState::Mode getMode() const { return getMode(this->getActiveBank()); }

    /// @copydoc    VPotState::getStartOn
    /// (For the active bank.)
    uint8_t getStartOn() const override {
        return getStartOn(this->getActiveBank());
    }
    /// @copydoc    VPotState::getStartOff
    /// (For the active bank.)
    uint8_t getStartOff() const override {
        return getStartOff(this->getActiveBank());
    }

    /// @}

    /// Reset all states to zero.
    void reset() override {
        if (!ignoreReset) {
            states = {{}};
            dirty = true;
        }
    }

  protected:
    void onBankSettingChange() override { dirty = true; }

  private:
    AH::Array<VPotState, BankSize> states = {{}};

  public:
    /// Don't reset the state when calling the `reset` method. This is the
    /// default, because in the original MCU, VPots don't get reset when a
    /// Reset All Controllers message is received.
    bool ignoreReset = true;
};

} // namespace Bankable

} // namespace MCU

END_CS_NAMESPACE
