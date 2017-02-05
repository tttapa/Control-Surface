#ifndef RotaryEncoder_h_
#define RotaryEncoder_h_

#include "USBMidi.h"
#include "Arduino.h"
#include "Encoder.h"

const static byte ADD_64 = 1;
const static byte SIGN_BIT = 2;
const static byte POS1_NEG127 = 3;

const static byte NORMAL_ENCODER = 4;
const static byte JOG = 1;

class RotaryEncoder
{
public:
  RotaryEncoder(byte p, byte p2, byte n, byte c, int sp, byte pps, byte m); // first pin, second pin, controller number, channel, speed multiplier, pulses  per step.
  void refresh();
private: 
  byte controller, channel, pulsesPerStep, mode;
  int speedMultiply, count;
  long value, oldVal, difference;
  Encoder enc;
};
#endif