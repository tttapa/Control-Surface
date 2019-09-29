#pragma once

#include <Hardware/LEDs/LEDs.hpp>
#include <MIDI_Inputs/MCU/VPotRing.hpp>

BEGIN_CS_NAMESPACE

namespace MCU {

class VPotRingLEDs {
  public:
    VPotRingLEDs(const LEDs<11> &leds) : leds(leds) {}

    template <class T>
    void begin(const T &) {
        leds.begin();
    }

    template <class T>
    void update(const T &t) {
        leds.displayRange(t.getStartOn(), t.getStartOff());
    }

  private:
    const LEDs<11> leds;
};

// -------------------------------------------------------------------------- //

// TODO

namespace Bankable {} // namespace Bankable

} // namespace MCU

END_CS_NAMESPACE