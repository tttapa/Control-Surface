#include "RotaryEncoder.h"
#include "Encoder.h"
#include "USBMidi.h"

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
void RotaryEncoder::refresh() {
  int msgVal = 0;
  value = enc.read();
  difference = (value - oldVal) / pulsesPerStep;
  if (difference > 63) difference = 63;
  if (difference < -63) difference = -63;
  if (difference != 0) {
    count = 0;
    if (difference < 0) {
      if (mode == ADD_64) {
        msgVal = 64 + difference * speedMultiply;
      } else if (mode == SIGN_BIT) {
        msgVal = bit(6) - difference * speedMultiply; // bit(6) gives 64, this is the sign bit
      } else if (mode == POS1_NEG127) {
        msgVal = 128 + difference * speedMultiply;
      }
      USBMidiController.send(CC, channel, controller, msgVal);
    }
    else if ( difference > 0) {
      if (mode == ADD_64) {
        msgVal = 64 + difference * speedMultiply;
      } else if (mode == SIGN_BIT) {
        msgVal = difference * speedMultiply;
      } else if (mode == POS1_NEG127) {
        msgVal = 0 + difference * speedMultiply;
      }
      USBMidiController.send(CC, channel, controller, msgVal);
    }
    oldVal = value;
  }
}