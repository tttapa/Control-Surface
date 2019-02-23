#pragma once

#include <Hardware/LEDs/VULEDs.hpp>
#include <MIDI_Inputs/MCU/VU.hpp>

namespace MCU {

template <uint8_t N>
class VULEDs_Base : virtual public VU_Base {
  protected:
    VULEDs_Base(const DotBarDisplayLEDs<N> &leds) : leds(leds) {}

  public:
    void begin() override { leds.begin(); }

  protected:
    void display() const override {
        uint8_t value = this->getValue();            // value in [0, 12]
        value = (value * N + FLOOR_CORRECTION) / 12; // value in [0, N]
        leds.display(value);
    }

  private:
    const DotBarDisplayLEDs<N> leds;

    /// @see    doc/VU-LED-mapping.ods
    constexpr static uint8_t FLOOR_CORRECTION = 5;
};

// -------------------------------------------------------------------------- //

class VULEDPWM_Base : virtual public VU_Base {
  protected:
    VULEDPWM_Base(pin_t ledPin) : ledPin(ledPin) {}

  public:
    void begin() override { pinMode(ledPin, OUTPUT); }

  protected:
    void display() const override {
        constexpr static uint8_t lut[13] = {0,  1,  3,  5,   8,   14, 21,
                                            33, 50, 75, 113, 170, 255};
        analogWrite(ledPin, lut[this->getValue()]);
    }

  private:
    const pin_t ledPin;
};

// -------------------------------------------------------------------------- //

/**
 * @brief 
 * 
 * @tparam  N
 *          Number of LEDs.
 */
template <uint8_t N>
class VULEDs : public VU, public VULEDs_Base<N> {
  public:
    VULEDs(const MIDICNChannelAddress &address,
           const DotBarDisplayLEDs<N> &leds, unsigned int decayTime = 150)
        : VU_Base(address, decayTime),
          VU(address, decayTime), VULEDs_Base<N>(leds) {}

  protected:
    void display() const override { VULEDs_Base<N>::display(); }
};

class VULEDPWM : public VU, public VULEDPWM_Base {
  public:
    VULEDPWM(const MIDICNChannelAddress &address, pin_t ledPin,
             unsigned int decayTime = 150)
        : VU_Base(address, decayTime), VU(address, decayTime),
          VULEDPWM_Base(ledPin) {}

  private:
    void display() const override { VULEDPWM_Base::display(); }
};

// -------------------------------------------------------------------------- //

namespace Bankable {

/**
 * @brief 
 * 
 * @todo    documentation 
 * 
 * @tparam  M
 *          Number of settings.
 * @tparam  N
 *          Number of LEDs.
 */
template <uint8_t M, uint8_t N>
class VULEDs : public VU<M>, public VULEDs_Base<N> {
  public:
    VULEDs(const BankConfig<M> &config, const DotBarDisplayLEDs<N> &leds,
           const MIDICNChannelAddress &address, unsigned int decayTime = 150)
        : VU_Base(address, decayTime), VU<M>(config, address, decayTime),
          VULEDs_Base<N>(leds) {}

  private:
    void display() const override { VULEDs_Base<N>::display(); }
    void onBankSettingChange() const override { VULEDs_Base<N>::display(); }
};

} // namespace Bankable

} // namespace MCU