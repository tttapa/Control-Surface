#ifndef CONTROL_SURFACE_MIDI_OUTPUTS_CCPOTENTIOMETER_HPP_
#define CONTROL_SURFACE_MIDI_OUTPUTS_CCPOTENTIOMETER_HPP_

#include "CCOut.h"
#include "../Hardware/FilteredAnalog.h"

class CCPotentiometer : public CCOut
{
  public:
    CCPotentiometer(pin_t analogPin, uint8_t address, uint8_t channel)
        : CCOut(address, channel), filteredAnalog{analogPin}
    {
    }

  private:
    void refresh()
    {
        if(filteredAnalog.update())
            send(filteredAnalog.getValue());
    }

    FilteredAnalog<3> filteredAnalog;
};

#endif // CONTROL_SURFACE_MIDI_OUTPUTS_CCPOTENTIOMETER_HPP_