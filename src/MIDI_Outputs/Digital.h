#ifndef DIGITAL_H
#define DIGITAL_H

#include "./MIDI_Control_Element.h"
#include "../Settings/SettingsWrapper.h"
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

  const static unsigned long debounceTime = BUTTON_DEBOUNCE_TIME;  // Edit this in ../Settings/Settings.h

  const static int8_t falling = LOW - HIGH;
  const static int8_t rising = HIGH - LOW;

  void setChannelOffset(uint8_t offset);
  void setAddressOffset(uint8_t offset);

  uint8_t newAddressOffset = addressOffset;
  uint8_t newChannelOffset = channelOffset;
};

#endif // DIGITAL_H
