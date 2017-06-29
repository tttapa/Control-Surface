#ifndef AnalogHiRes_h_
#define AnalogHiRes_h_

#include "Arduino.h"
#include "USBMidi.h"

class AnalogHiRes // See instructable on how to use classes
{
public:
  AnalogHiRes(byte p, byte c); // pin, channel
  ~AnalogHiRes();
  void average(size_t len); // length of array of samples
  void refresh();
  void map(uint16_t (*fn)(uint16_t));
  void bank(byte dPin,byte newC); // digital pin, new channel
  void detachBank();
private:
  byte analogPin, channel, digitalPin, newChannel;
  uint16_t value;
  int16_t oldVal;
  boolean bankTrue = false;
  uint16_t (*AnalogHiResMap)(uint16_t) = [](uint16_t x){return x;};
  size_t av = 0;
  unsigned int* avValues;
  byte avIndex = 0;
  unsigned long long avSum = 0;
  byte avCount = 0;
  unsigned int runningAverage(int M); // http://playground.arduino.cc/Main/RunningAverage
};

#endif // AnalogHiRes_h_
