#pragma once

#include "../../Hardware/ExtendedInputOutput/ExtendedInputOutput.h"
#include "../../Helpers/Copy.hpp"
#include "../MCU_VPotRing.hpp"

class MCU_VPotRing_LEDs_Base : public virtual MCU_VPotRing_Base {
  public:
    MCU_VPotRing_LEDs_Base(const pin_t (&ledPins)[11]) {
        copy(this->ledPins, ledPins);
        for (const pin_t &pin : ledPins)
            ExtIO::pinMode(pin, OUTPUT);
    }

  private:
    void display() const {
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

class MCU_VPotRing_LEDs : public MCU_VPotRing, public MCU_VPotRing_LEDs_Base {
  public:
    MCU_VPotRing_LEDs(uint8_t track, uint8_t channel,
                      const pin_t (&ledPins)[11])
        : MCU_VPotRing(track, channel), MCU_VPotRing_LEDs_Base(ledPins) {}
};

// -------------------------------------------------------------------------- //

namespace Bankable {

template <uint8_t NUMBER_OF_BANKS>
class MCU_VPotRing_LEDs : public MCU_VPotRing<NUMBER_OF_BANKS>,
                          public MCU_VPotRing_LEDs_Base {
  public:
    MCU_VPotRing_LEDs(uint8_t track, uint8_t channel,
                      const pin_t (&ledPins)[11])
        : MCU_VPotRing<NUMBER_OF_BANKS>(track, channel),
          MCU_VPotRing_LEDs_Base(ledPins) {}
};

} // namespace Bankable