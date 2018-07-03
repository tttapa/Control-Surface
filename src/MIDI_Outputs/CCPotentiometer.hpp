#pragma once

#include "../Hardware/FilteredAnalog.h"
#include "Abstract/CCOut.h"

class CCPotentiometer : public CCOut {
  public:
    CCPotentiometer(pin_t analogPin, uint8_t address, uint8_t channel)
        : CCOut(address, channel), filteredAnalog{analogPin} {}

  private:
    void refresh() {
        if (filteredAnalog.update())
            send(filteredAnalog.getValue());
    }

    FilteredAnalog<7> filteredAnalog; // 7 bits of precision
};