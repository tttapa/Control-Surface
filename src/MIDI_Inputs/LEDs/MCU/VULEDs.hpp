#pragma once

#include <Hardware/LEDs/DotBarDisplayLEDs.hpp>
#include <MIDI_Inputs/MCU/VU.hpp>

BEGIN_CS_NAMESPACE

namespace MCU {

template <uint8_t NumLEDs>
class VULEDsCallback {
  public:
    VULEDsCallback(const DotBarDisplayLEDs<NumLEDs> &leds) : leds(leds) {}

    template <class T>
    void begin(T &) {
        leds.begin();
    }

    template <class T>
    void update(T &t) {
        uint8_t value = t.getValue();                      // value in [0, 12]
        value = (value * NumLEDs + FLOOR_CORRECTION) / 12; // value in [0, N]
        leds.display(value);
    }

  private:
    const DotBarDisplayLEDs<NumLEDs> leds;

    /// @see    doc/VU-LED-mapping.ods
    constexpr static uint8_t FLOOR_CORRECTION = 5;
};

template <uint8_t NumLEDs>
using VULEDs = VU_Generic<VULEDsCallback<NumLEDs>>;

namespace Bankable {

template <uint8_t NumBanks, uint8_t NumLEDs>
using VULEDs = VU_Generic<NumBanks, VULEDsCallback<NumLEDs>>;

} // namespace Bankable

} // namespace MCU

END_CS_NAMESPACE