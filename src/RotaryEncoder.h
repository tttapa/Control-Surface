#ifndef RotaryEncoder_h_
#define RotaryEncoder_h_

#include "USBMidi.h"
#include "Arduino.h"
#include "Encoder.h"

enum relativeCCmode
{
  TWOS_COMPLEMENT,
  BINARY_OFFSET,
  SIGN_MAGNITUDE
};

#define REAPER_RELATIVE_1 TWOS_COMPLEMENT
#define REAPER_RELATIVE_2 BINARY_OFFSET
#define REAPER_RELATIVE_3 SIGN_MAGNITUDE

#define TRACKTION_RELATIVE TWOS_COMPLEMENT

#define MACKIE_CONTROL_RELATIVE SIGN_MAGNITUDE

#define POS1_NEG127 TWOS_COMPLEMENT
#define ADD_64 BINARY_OFFSET
#define SIGN_BIT SIGN_MAGNITUDE

const uint8_t NORMAL_ENCODER = 4; // A normal rotary encoder sends four pulses per physical 'click'
const uint8_t JOG = 1;            // For jog wheels, you want the highest possible resolution

class RotaryEncoder
{
public:
  RotaryEncoder(uint8_t pinA, uint8_t pinB, uint8_t controllerNumber, uint8_t channel, int speedMultiply, uint8_t pulsesPerStep, relativeCCmode mode); // Constructor
  ~RotaryEncoder();                                                                                                                                    // Deconstructor
  void refresh();                                                                                                                                      // Check if the encoder position has changed since last time, if so, send the relative movement over MIDI
  void bank(uint8_t bankPin, uint8_t altController, uint8_t altChannel);                                                                               // Enable the bank mode. When bank switch is turned on, send alternative MIDI channel and controller numbers
  void detachBank();                                                                                                                                   // Disable the bank mode

private:
  uint8_t controllerNumber, channel, altController, altChannel, pulsesPerStep, bankPin;
  bool bankEnabled = false;
  relativeCCmode mode;
  int speedMultiply;
  long oldVal = 0;
  Encoder enc;

  uint8_t twosComplement7bit(int8_t value);                  // Convert an 8-bit two's complement integer to a 7-bit two's complement integer
  uint8_t twosComplementTo7bitSignedMagnitude(int8_t value); // Convert an 8-bit two's complement integer to 7-bit sign-magnitude format
  uint8_t mapRelativeCC(int8_t value, relativeCCmode type);  // Convert an 8-bit two's complement integer to a 7-bit value to send over MIDI
};
#endif