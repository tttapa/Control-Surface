#ifndef DIG_h_
#define DIG_h_

#include "Arduino.h"

class Digital // See instructable on how to use classes
{
public:
  Digital(byte p, byte n, byte c, byte v); // pin, controller number, channel, velocity
  void refresh();
  void bank(byte bPin, byte newN, byte newC); // digital bank pin, new controller number, new channel
  void detachBank();
private:
  byte pin, note, channel, value, oldVal, velocity, bankPin, newNote, newChannel;
  boolean validPin;
  boolean bankTrue;
};

#endif
