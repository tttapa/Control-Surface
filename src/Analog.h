#ifndef ANALOG_h_
#define ANALOG_h_

#include "Arduino.h"
#include "USBMidi.h"

class Analog // See instructable on how to use classes
{
public:
  Analog(byte p, byte n, byte c); // pin, controller number, channel
  Analog(byte p, byte n, byte c, byte r); // pin, controller number, channel, (resolution) for compatibility only, will be removed in next revision  
  ~Analog();
  void average(size_t len); // length of array of samples
  void refresh();
  void bank(byte dPin, byte newN, byte newC); // digital pin, new controller number, new channel
  void detachBank();
private:
  byte analogPin, controller, channel, digitalPin, newController, newChannel, value, oldVal;
  boolean bankTrue = false;
  size_t av = 0;
  unsigned int* avValues;
  byte avIndex = 0;
  unsigned long avSum = 0;
  byte avCount = 0;
  unsigned int runningAverage(int M); // http://playground.arduino.cc/Main/RunningAverage
};

#endif // ANALOG_h_
