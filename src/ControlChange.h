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
  ControlChange(uint8_t n, uint8_t c); // Constructor (MIDI controller number, MIDI channel)
  ~ControlChange(); // deconstructor
  void average(size_t len); // length of array of samples
  void refresh(uint8_t input); // update the object with new value, send if value changed (value between 0 and 127 (0x7F))
  void refresh(float input); // update the object with new value, send if value changed (value between 0.0 and 1.0)
  void bank(uint8_t bPin, uint8_t newN, uint8_t newC); // set up a bank ((digital) bank pin, new controller number, new channel)
  void detachBank(); // disable the bank
private:
  uint8_t controller, channel, bankPin, newController, newChannel, value;
  int oldVal;
  bool bankEnabled = false;
  size_t av = 0;
  unsigned int* avValues;
  uint8_t avIndex = 0;
  unsigned long avSum = 0;
  uint8_t avCount = 0;
  unsigned int runningAverage(int M); // http://playground.arduino.cc/Main/RunningAverage
};

#endif // ControlChange_h_
