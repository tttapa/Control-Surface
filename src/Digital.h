#ifndef DIG_h_
#define DIG_h_

#include "Arduino.h"
#include "USBMidi.h"

class Digital
{
public:
  Digital(uint8_t pin, uint8_t note, uint8_t channel, uint8_t velocity); // Constructor
  ~Digital();                                                            // Deconstructor
  void refresh();                                                        // Check if the button state changed, and send a MIDI Note On or Off accordingly
  void bank(uint8_t bankPin, uint8_t altNote, uint8_t altChannel);       // Enable the bank mode. When bank switch is turned on, send alternative MIDI channel and note numbers
  void detachBank();                                                     // Disable the bank mode
  boolean invert = false;                                                // invert the button state (send Note On event when released, Note Off when pressed)
private:
  uint8_t pin, note, channel, velocity, bankPin, altNote, altChannel, oldVal = -1;
  bool bankEnabled = false;

  void sendNote(uint8_t noteOnOrOff); // turn on or off a note, select the channel and note number based on the bank mode and bank switch state
};

#endif
