#ifndef CONTROL_SURFACE_MIDI_OUTPUTS_PBPOTENTIOMETER_HPP_
#define CONTROL_SURFACE_MIDI_OUTPUTS_PBPOTENTIOMETER_HPP_

#include "FilteredAnalog.h"
#include "PBOut.h"

class PBPotentiometer : public PBOut {
public:
  PBPotentiometer(pin_t analogPin, uint8_t channel)
      : PBOut(channel), filteredAnalog{analogPin} {}

private:
  void refresh() {
    if (filteredAnalog.update())
      send(filteredAnalog.getValue() << 6);
  }

  FilteredAnalog<2> filteredAnalog;
};

#endif // CONTROL_SURFACE_MIDI_OUTPUTS_PBPOTENTIOMETER_HPP_