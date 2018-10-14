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

    void display() const override {
        uint8_t value = this->getValue();            // value in [0, 12]
        value = (value * N + FLOOR_CORRECTION) / 12; // value in [0, N]
        leds.display(value);
    }

  private:
    const DotBarDisplayLEDs<N> leds;

    /** @see    doc/VU-LED-mapping.ods */
    constexpr static uint8_t FLOOR_CORRECTION = 5;
};

/**
 * @brief 
 * 
 * @tparam  N
 *          Number of LEDs.
 */
template <uint8_t N>
class VULEDs : public VU, public VULEDs_Base<N> {
  public:
    VULEDs(uint8_t track, Channel channel, const DotBarDisplayLEDs<N> &leds,
           unsigned int decayTime = 150)
        : VU_Base(track, channel, decayTime),
          VU(track, channel), VULEDs_Base<N>(leds) {}
};

namespace Bankable {

/**
 * @brief 
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
    void onBankSettingChange() const override { this->display(); }
};

} // namespace Bankable

} // namespace MCU