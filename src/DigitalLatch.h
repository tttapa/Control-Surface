#ifndef DIGLAT_h_
#define DIGLAT_h_

#include "Arduino.h"
#include "USBMidi.h"

class DigitalLatch
{
public:
  DigitalLatch(uint8_t pin, uint8_t note, uint8_t channel, uint8_t velocity, unsigned long latchTime); // Constructor
  ~DigitalLatch();                                                                                     // Deconstructor
  void refresh();                                                                                      // Check if the button state changed, if so, send a MIDI Note On, after a non-blocking delay of "latchTime", send a Note Off
  void bank(uint8_t bankPin, uint8_t altNote, uint8_t altChannel);                                     // Enable the bank mode. When bank switch is turned on, send alternative MIDI channel and controller numbers
  void detachBank();                                                                                   // Disable the bank mode

private:
  uint8_t pin, note, channel, velocity, bankPin, altNote, altChannel, oldState = -1;
  bool noteOffSent = true;
  bool bankEnabled = false;
  unsigned long latchTime;
  unsigned long noteOnTime;

  void sendNote(uint8_t noteOnOrOff); // turn on or off a note, select the channel and note number based on the bank mode and bank switch state 
};

#endif
