#pragma once

#include <Hardware/LEDs/LEDs.hpp>
#include <MIDI_Inputs/MCU/VPotRing.hpp>

namespace MCU {

class VPotRingLEDs_Base : public virtual VPotRing_Base {
  public:
    VPotRingLEDs_Base(const LEDs<11> &leds) : leds(leds) {}

    void begin() override {
        leds.begin();
    }

    void display() const override {
        leds.displayRange(getStartOn(), getStartOff());
    }

  private:
    const LEDs<11> leds;
};

// -------------------------------------------------------------------------- //

class VPotRingLEDs : public VPotRing, public VPotRingLEDs_Base {
  public:
    VPotRingLEDs(uint8_t track, uint8_t channel, const LEDs<11> &leds)
        : VPotRing_Base(track, channel), VPotRing(track, channel),
          VPotRingLEDs_Base(leds) {}
};

// -------------------------------------------------------------------------- //

namespace Bankable {

template <uint8_t N>
class VPotRingLEDs : public VPotRing<N>,
                     public VPotRingLEDs_Base {
  public:
    VPotRingLEDs(const BankConfigAddressable<N> &config, uint8_t track,
                 uint8_t channel, const LEDs<11> &leds)
        : VPotRing_Base(track, channel), VPotRing<N>(
                                             config, track, channel),
          VPotRingLEDs_Base(leds) {}
};

} // namespace Bankable

} // namespace MCU