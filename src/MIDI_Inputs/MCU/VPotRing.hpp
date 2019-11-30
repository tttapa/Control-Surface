#pragma once

#include <Banks/BankableMIDIInput.hpp>
#include <MIDI_Inputs/MIDIInputElementCC.hpp>

BEGIN_CS_NAMESPACE

namespace MCU {

constexpr static uint8_t VPotRingAddress = 0x30;

inline int8_t minimum(int8_t a, int8_t b) { return a > b ? b : a; }
inline int8_t maximum(int8_t a, int8_t b) { return a < b ? b : a; }

struct VPotEmptyCallback {
    VPotEmptyCallback() = default;
    template <class T>
    void begin(const T &) {}
    template <class T>
    void update(const T &) {}
};

/**
 * @todo    I'm terrible at naming things.
 */
class IVPotRing {
  protected:
    IVPotRing() = default;

  public:
    /// Return the position of the V-Pot ring. [0, 11]
    uint8_t getPosition() const { return getPosition(getValue()); }
    /// Return the status of the center LED of the V-Pot ring.
    bool getCenterLed() const { return getCenterLed(getValue()); }
    /// Return the mode of the V-Pot ring: 0 = single dot, 1 = boost/cut,
    /// 2 = wrap, 3 = spread
    uint8_t getMode() const { return getMode(getValue()); }

    /// Get the first segment that should be on.
    uint8_t getStartOn() const {
        int8_t position = getPosition();
        if (position == 0)
            return 0;
        int8_t value = position - 1;
        switch (getMode()) {
            case 0: return value;
            case 1: return minimum(value, 5);
            case 2: return 0;
            case 3: return maximum(5 - value, 0);
            // Shouldn't happen, just keeps the compiler happy:
            default: return 0;
        }
    }

    /// Get the first segment that should be off.
    uint8_t getStartOff() const {
        uint8_t value = getPosition();
        switch (getMode()) {
            case 0: return value;
            case 1: return maximum(value, 6);
            case 2: return value;
            case 3: return minimum(5 + value, 11);
            // Shouldn't happen, just keeps the compiler happy:
            default: return 0;
        }
    }

  private:
    virtual uint8_t getValue() const = 0;

    /// Extract the position from the raw value.
    static uint8_t getPosition(uint8_t value) {
        uint8_t position = value & 0x0F;
        return position < 0x0C ? position : 0x0B;
    }
    /// Extract the center LED state from the raw value.
    static bool getCenterLed(uint8_t value) { return value & 0x40; }
    /// Extract the mode from the raw value.
    static uint8_t getMode(uint8_t value) { return (value & 0x30) >> 4; }
};

template <uint8_t NumValues, class Callback>
class VPotRing_Base : public MIDIInputElementCC, public IVPotRing {
  protected:
    VPotRing_Base(uint8_t track, const MIDICNChannel &channelCN,
                  const Callback &callback)
        : MIDIInputElementCC{{track + VPotRingAddress - 1, channelCN}},
          callback(callback) {}

  public:
    /// Initialize
    void begin() override { callback.begin(*this); }

    /// Reset all values to zero
    void reset() override {
#ifdef VPOTRING_RESET
        values = {{}};
        callback.update(*this);
#endif
    }

  protected:
    /** Make sure that the received value is valid and will not result in array
     * out of bounds conditions. */
    static uint8_t sanitizeValue(uint8_t value) {
        return (value & 0x0F) < 0x0C ? value : ((value & 0xF0) | 0xB);
    }

  private:
    bool updateImpl(const ChannelMessageMatcher &midimsg,
                    const MIDICNChannelAddress &target) override {
        uint8_t index = getBankIndex(target);
        uint8_t value = sanitizeValue(midimsg.data2);
        values[index] = value;
        return true;
    }

    uint8_t getValue() const override { return values[getSelection()]; }

    /// Get the active bank selection
    virtual uint8_t getSelection() const { return 0; }

    /// Get the bank index from a MIDI address
    virtual setting_t getBankIndex(const MIDICNChannelAddress &target) const {
        (void)target;
        return 0;
    }

    Array<uint8_t, NumValues> values = {{}};

  public:
    Callback callback;
};

// -------------------------------------------------------------------------- //

/** 
 * @brief   A class for MIDI input elements that represent Mackie Control
 *          Universal V-Pots. This version is generic to allow for custom 
 *          callbacks.  
 *          This version cannot be banked.
 */
template <class Callback = VPotEmptyCallback>
class GenericVPotRing : public VPotRing_Base<1, Callback> {
  public:
    GenericVPotRing(uint8_t track, const MIDICNChannel &channelCN,
                    const Callback &callback)
        : VPotRing_Base<1, Callback>{track, channelCN, callback} {}
};

/**
 * @brief   A class for MIDI input elements that represent Mackie Control
 *          Universal V-Pots.  
 *          This version cannot be banked.
 * @ingroup MIDIInputElements
 */
class VPotRing : public GenericVPotRing<> {
  public:
    VPotRing(uint8_t track, const MIDICNChannel &channelCN = CHANNEL_1)
        : GenericVPotRing{track, channelCN, {}} {}
};

// -------------------------------------------------------------------------- //

namespace Bankable {

/** 
 * @brief   A class for MIDI input elements that represent Mackie Control
 *          Universal V-Pots. This version is generic to allow for custom 
 *          callbacks.  
 *          This version can be banked.
 * 
 * @tparam  NumBanks 
 *          The number of banks.
 */
template <uint8_t NumBanks, class Callback = VPotEmptyCallback>
class GenericVPotRing : public VPotRing_Base<NumBanks, Callback>,
                        public BankableMIDIInput<NumBanks> {
  public:
    GenericVPotRing(const BankConfig<NumBanks> &config, uint8_t track,
                    const MIDICNChannel &channelCN, const Callback &callback)
        : VPotRing_Base<NumBanks, Callback>{track, channelCN, callback},
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
 *          Universal V-Pots.  
 *          This version can be banked.
 * 
 * @tparam  NumBanks 
 *          The number of banks.
 */
template <uint8_t NumBanks>
class VPotRing : public GenericVPotRing<NumBanks> {
  public:
    VPotRing(BankConfig<NumBanks> config, uint8_t track,
             MIDICNChannel channelCN = CHANNEL_1)
        : GenericVPotRing<NumBanks>{config, track, channelCN, {}} {}
};

} // namespace Bankable

} // namespace MCU

END_CS_NAMESPACE