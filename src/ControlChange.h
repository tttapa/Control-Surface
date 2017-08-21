#ifndef ControlChange_h_
#define ControlChange_h_

#include "Arduino.h"
#include "USBMidi.h"

/* Class for sending control change MIDI messages.
   Features:
   * Only send data if value changes
   * Averaging of input data supported
   * Split the controller up in banks (send 
     different controller messages or channels 
     depending on the state of a bank switch)
*/

class ControlChange
{
public:
  ControlChange(uint8_t controllerNumber, uint8_t channel);              // Constructor
  ~ControlChange();                                                      // Destructor
  void average(size_t len);                                              // length of array of samples
  void refresh(uint8_t value);                                           // Update the controller with a new value between 0 and 127, update the average, check if the MIDI value changed since last time, if so, send a Control Change message over MIDI
  void refresh(float input);                                             // Overload of the refresh() method that takes a float between 0.0 and 1.0 instead of an int between 0 and 127
  void bank(uint8_t bankPin, uint8_t altController, uint8_t altChannel); // Enable the bank mode. When bank switch is turned on, send alternative MIDI channel and controller numbers
  void detachBank();                                                     // disable the bank

private:
  uint8_t controllerNumber, channel, bankPin, altController, altChannel, value, oldVal = -1;
  bool bankEnabled = false;
  size_t avLen = 0;
  unsigned int *avValues;
  uint8_t avIndex = 0;
  unsigned long avSum = 0;
  uint8_t avCount = 0;
  unsigned int runningAverage(unsigned int value); // http://playground.arduino.cc/Main/RunningAverage
};

#endif // ControlChange_h_
