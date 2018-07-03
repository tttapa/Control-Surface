#pragma once

#include "../Hardware/FilteredAnalog.h"
#include "Abstract/PBOut.h"

class PBPotentiometer : public PBOut {
  public:
    PBPotentiometer(pin_t analogPin, uint8_t channel)
        : PBOut(channel), filteredAnalog{analogPin} {}

  private:
    void refresh() {
        if (filteredAnalog.update())
            send(filteredAnalog.getValue() << 6);
    }

    FilteredAnalog<8>
        filteredAnalog; // 8 bits of precision (i.e. 2 least significant bits
                        // are used for filtering)
};