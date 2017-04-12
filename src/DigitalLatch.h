#ifndef DIGLAT_h_
#define DIGLAT_h_

#include "Arduino.h"
#include "USBMidi.h"

class DigitalLatch // See instructable on how to use classes
{
public:
  DigitalLatch(byte p, byte n, byte c, byte v, unsigned int l); // pin, controller number, channel, latch time (ms)
  void refresh();
  void bank(byte bPin, byte newN, byte newC);
  void detachBank();
private:
  byte pin, note, channel, value, oldVal, velocity, bankPin, newNote, newChannel;
  boolean offSent;
  boolean bankTrue;
  unsigned int latch;
  unsigned long time;
};

#endif
