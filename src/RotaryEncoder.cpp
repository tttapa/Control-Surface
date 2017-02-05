#include "RotaryEncoder.h"
#include "Encoder.h"

RotaryEncoder::RotaryEncoder(byte p, byte p2, byte n, byte c, int sp, byte pps, byte m) : enc(p,p2) // first pin, second pin, controller number, channel, speed multiplier, pulses  per step.
{
  controller = n;
  channel = c;
  speedMultiply = sp;
  oldVal = 0;
  pulsesPerStep = pps;
  count = pps - 1;
  mode = m;
}
void RotaryEncoder::refresh(MIDISender& s) {
  int msgVal = 0;
  value = enc.read();
  difference = (value - oldVal) / pulsesPerStep;
  if (difference > 63) difference = 63;
  if (difference < -63) difference = -63;
  if (difference != 0) {
    count = 0;
    if (difference < 0) {
      if (mode == 1) {
        msgVal = 63 + difference * speedMultiply;
      } else if (mode == 2) {
        msgVal = bit(6) - difference * speedMultiply; // bit(6) gives 64, this is the sign bit
      } else if (mode == 3) {
        msgVal = 128 + difference * speedMultiply;
      }
      USB.send(CC, channel, controller, msgVal);
    }
    else if ( difference > 0) {
      if (mode == 1) {
        msgVal = 63 + difference * speedMultiply;
      } else if (mode == 2) {
        msgVal = difference * speedMultiply;
      } else if (mode == 3) {
        msgVal = 0 + difference * speedMultiply;
      }
      USB.send(CC, channel, controller, msgVal);
    }
    oldVal = value;
  }
}