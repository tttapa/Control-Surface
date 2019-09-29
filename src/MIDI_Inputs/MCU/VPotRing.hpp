#pragma once

#include <MIDI_Inputs/MIDIInputElementCC.hpp>
#include <MIDI_Inputs/MIDINoteCCRange.hpp>

BEGIN_CS_NAMESPACE

#define CRTP(Derived) (*static_cast<Derived *>(this))

namespace MCU {

constexpr static uint8_t VPotRingAddress = 0x30;

inline int8_t minimum(int8_t a, int8_t b) { return a > b ? b : a; }
inline int8_t maximum(int8_t a, int8_t b) { return a < b ? b : a; }

struct VPotEmptyCallback {
    template <class T>
    void begin(const T &) {}
    template <class T>
    void update(const T &) {}
};

template <uint8_t NumValues, class Callback, class Derived>
class VPotRing_Base : public MIDIInputElementCC {
  public:
    VPotRing_Base(uint8_t track, const MIDICNChannel &channelCN,
                  const Callback &callback)
        : MIDIInputElementCC{{address + VPotRingAddress - 1, channelCN}},
          callback{callback} {}

  public:
    /// Initialize
    void begin() override { callback.begin(*this); }
    /// Reset all values to zero
    void reset() override {
        values = {};
        callback.update(*this);
    }

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

    uint8_t getValue() const { return values[getSelection()]; }

    uint8_t getSelection() const { return CRTP(Derived).getSelection(); }
    uint8_t getBankIndex(uint8_t target) const {
        return CRTP(Derived).getBankIndex(target, this->address);
    }

    /// Extract the position from the raw value.
    static uint8_t getPosition(uint8_t value) {
        uint8_t position = value & 0x0F;
        return position < 0x0C ? position : 0x0B;
    }
    /// Extract the center LED state from the raw value.
    static bool getCenterLed(uint8_t value) { return value & 0x40; }
    /// Extract the mode from the raw value.
    static uint8_t getMode(uint8_t value) { return (value & 0x30) >> 4; }

    Array<uint8_t, NumValues> values = {};

  public:
    Callback callback;
};

// -------------------------------------------------------------------------- //

/** 
 * @brief   A class for MIDI input elements that represent Mackie Control
 *          Universal V-Pots.  
 *          This version cannot be banked.
 */
template <class Callback = VPotEmptyCallback>
class VPotRing : virtual public VPotRing_Base<1, Callback, VPotRing<Callback>> {
  public:
    VPotRing(uint8_t track, const MIDICNChannel &channelCN,
             const Callback &callback = {})
        : VPotRing_Base<1, Callback, VPotRing<Callback>>{{track, channelCN},
                                                         callback} {}

  private:
    setting_t getSelection() const { return 0; }
    uint8_t getBankIndex(const MIDICNChannelAddress &target,
                         const MIDICNChannelAddress &base) const {
        (void)target;
        (void)base;
        return 0;
    }
};

// -------------------------------------------------------------------------- //

namespace Bankable {

/** 
 * @brief   A class for MIDI input elements that represent Mackie Control
 *          Universal V-Pots.  
 *          This version can be banked.
 * 
 * @tparam  NumBanks 
 *          The number of banks.
 */
template <uint8_t NumBanks, class Callback = VPotEmptyCallback>
class VPotRing : virtual public VPotRing_Base<NumBanks, Callback,
                                              VPotRing<NumBanks, Callback>>,
                 public BankableMIDIInput<NumBanks> {
  public:
    VPotRing(const BankConfig<NumBanks> &config, uint8_t track,
             const MIDICNChannel &channelCN, const Callback &callback = {})
        : VPotRing_Base<NumBanks, Callback, VPotRing<NumBanks, Callback>>(
              track, channelCN),
          BankableMIDIInput<NumBanks>(config) {}

  private:
    bool match(const MIDICNChannelAddress &target) const override {
        return BankableMIDIInput<NumBanks>::matchBankable(target,
                                                          this->address);
    }

    void onBankSettingChange() override { this->callback.update(); }
};

} // namespace Bankable

} // namespace MCU

#undef CRTP

END_CS_NAMESPACE