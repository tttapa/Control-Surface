#ifndef DIG_h_
#define DIG_h_

#include "Arduino.h"

#include "MIDI_Control_Element.h"

class Digital : public MIDI_Control_Element
{
public:
  Digital(uint8_t pin, uint8_t note, uint8_t channel, uint8_t velocity); // Constructor
  ~Digital();                                                            // Destructor
  void refresh();                                                        // Check if the button state changed, and send a MIDI Note On or Off accordingly
  boolean invert = false;                                                // invert the button state (send Note On event when released, Note Off when pressed)

private:
  uint8_t pin, note, channel, velocity, oldVal = -1;
};

#endif
