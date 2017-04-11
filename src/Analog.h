#ifndef ANALOG_h_
#define ANALOG_h_

#include "Arduino.h"
#include "USBMidi.h"

class Analog // See instructable on how to use classes
{
public:
  Analog(byte p, byte n, byte c); // pin, controller number, channel
  Analog(byte p, byte n, byte c, byte r); // pin, controller number, channel, (resolution) for compatibility only, will be removed in next revision  
  void refresh();
  void bank(byte dPin, byte newN, byte newC); // digital pin, new controller number, new channel
  void detachBank();
private:
  byte analogPin, controller, channel, digitalPin, newController, newChannel, value, oldVal;
  boolean bankTrue = false;
#ifdef ANALOG_AVERAGE
  int avValues[ANALOG_AVERAGE];
  byte avIndex = 0;
  long avSum = 0;
  byte avCount = 0;
  long runningAverage(int M) { // http://playground.arduino.cc/Main/RunningAverage
    // keep sum updated to improve speed.
    avSum -= avValues[avIndex];
    avValues[avIndex] = M;
    avSum += avValues[avIndex];
    avIndex++;
    avIndex = avIndex % ANALOG_AVERAGE;
    if (avCount < ANALOG_AVERAGE) avCount++;

    return avSum / avCount;
  }
#endif // ANALOG_AVERAGE
};

#endif // ANALOG_h_
