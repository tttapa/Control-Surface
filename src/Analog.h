#ifndef ANALOG_h_
#define ANALOG_h_

#include "Arduino.h"
#include "sendMidi.h"

class Analog // See instructable on how to use classes
{
public:
  Analog(byte p, byte n, byte c, byte r); // pin, controller number, channel, resolution  
  void refresh();
  void bank(byte dPin, byte newN, byte newC); // digital pin, new controller number, new channel
  void detachBank();
private:
  byte analogPin, controller, channel, digitalPin, newController, newChannel, value, oldVal, resolution;
  boolean bankTrue;
};

#endif
