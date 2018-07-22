#pragma once

#include <Hardware/ExtendedInputOutput/ExtendedInputOutput.h>
#include <Helpers/Copy.hpp>
#include <MIDI_Inputs/MCU/VPotRing.hpp>

namespace MCU {

class VPotRingLEDs_Base : public virtual VPotRing_Base {
  public:
    VPotRingLEDs_Base(const pin_t (&ledPins)[11]) {
        copy(this->ledPins, ledPins);
        for (const pin_t &pin : ledPins)
            ExtIO::pinMode(pin, OUTPUT);
    }

  private:
    void display() const {
        DEBUGFN("");
        uint8_t startOn = getStartOn();
        uint8_t startOff = getStartOff();
        for (uint8_t pin = 0; pin < startOn; pin++)
            ExtIO::digitalWrite(ledPins[pin], LOW);
        for (uint8_t pin = startOn; pin < startOff; pin++)
            ExtIO::digitalWrite(ledPins[pin], HIGH);
        for (uint8_t pin = startOff; pin < 11; pin++)
            ExtIO::digitalWrite(ledPins[pin], LOW);
    }

  private:
    pin_t ledPins[11];
};

// -------------------------------------------------------------------------- //

class VPotRingLEDs : public VPotRing, public VPotRingLEDs_Base {
  public:
    VPotRingLEDs(uint8_t track, uint8_t channel, const pin_t (&ledPins)[11])
        : VPotRing_Base(track, channel), VPotRing(track, channel),
          VPotRingLEDs_Base(ledPins) {}
};

// -------------------------------------------------------------------------- //

namespace Bankable {

template <uint8_t NUMBER_OF_BANKS>
class VPotRingLEDs : public VPotRing<NUMBER_OF_BANKS>,
                     public VPotRingLEDs_Base {
  public:
    VPotRingLEDs(uint8_t track, uint8_t channel, const pin_t (&ledPins)[11])
        : VPotRing_Base(track, channel), VPotRing<NUMBER_OF_BANKS>(track,
                                                                   channel),
          VPotRingLEDs_Base(ledPins) {}
};

} // namespace Bankable

} // namespace MCU