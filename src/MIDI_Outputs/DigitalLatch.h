#ifndef DIGLAT_h_
#define DIGLAT_h_

#include "Arduino.h"
#include "MIDI_Control_Element.h"
#include "../ExtendedInputOutput/ExtendedInputOutput.h"

class DigitalLatch : public MIDI_Control_Element
{
public:
  DigitalLatch(pin_t pin, uint8_t note, uint8_t channel, uint8_t velocity, unsigned long latchTime); // Constructor
  ~DigitalLatch();                                                                                     // Destructor

private:
  void refresh(); // Check if the button state changed, if so, send a MIDI Note On, after a non-blocking delay of "latchTime", send a Note Off

  pin_t pin;
  uint8_t note, channel, velocity;
  bool oldState = HIGH;
  bool noteOffSent = true;
  unsigned long latchTime;
  unsigned long noteOnTime;
};

#endif
