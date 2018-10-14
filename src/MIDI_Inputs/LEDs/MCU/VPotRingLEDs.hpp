#pragma once

#include <Hardware/LEDs/LEDs.hpp>
#include <MIDI_Inputs/MCU/VPotRing.hpp>

namespace MCU {

class VPotRingLEDs_Base : virtual public VPotRing_Base {
  public:
    VPotRingLEDs_Base(const LEDs<11> &leds) : leds(leds) {}

    void begin() override { leds.begin(); }

    void display() const override {
        leds.displayRange(getStartOn(), getStartOff());
    }

  private:
    const LEDs<11> leds;
};

// -------------------------------------------------------------------------- //

class VPotRingLEDs : public VPotRing, public VPotRingLEDs_Base {
  public:
    VPotRingLEDs(const MIDICNChannelAddress &address, const LEDs<11> &leds)
        : VPotRing_Base(address), VPotRing(address), VPotRingLEDs_Base(leds) {}
};

// -------------------------------------------------------------------------- //

namespace Bankable {

template <uint8_t N>
class VPotRingLEDs : public VPotRing<N>, public VPotRingLEDs_Base {
  public:
    VPotRingLEDs(const BankConfig<N> &config,
                 const MIDICNChannelAddress &address, const LEDs<11> &leds)
        : VPotRing_Base(address), VPotRing<N>(config, address),
          VPotRingLEDs_Base(leds) {}
};

} // namespace Bankable

} // namespace MCU