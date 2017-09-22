#ifndef DIG_h_
#define DIG_h_

#include "Arduino.h"
#include "MIDI_Control_Element.h"
#include "../Settings/Settings.h"
#include "../ExtendedInputOutput/ExtendedInputOutput.h"

class Digital : public MIDI_Control_Element
{
public:
  Digital(pin_t pin, uint8_t note, uint8_t channel, uint8_t velocity = 127); // Constructor
  ~Digital();                                                                // Destructor
  void invert();                                                             // Invert the button state (send Note On event when released, Note Off when pressed)

private:
  void refresh(); // Check if the button state changed, and send a MIDI Note On or Off accordingly

  pin_t pin;
  uint8_t note, channel, velocity;
  bool prevState = HIGH, buttonState = HIGH;
  unsigned long prevBounceTime = 0;

  bool invertState = false;

  const unsigned long debounceTime = BUTTON_DEBOUNCE_TIME;

  const int8_t falling = LOW - HIGH;
  const int8_t rising = HIGH - LOW;
};

#endif
