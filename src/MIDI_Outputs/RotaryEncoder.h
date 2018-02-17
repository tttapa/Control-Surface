#ifndef RotaryEncoder_h_
#define RotaryEncoder_h_

// #include <Arduino.h>
#include <Encoder.h>
#include "./MIDI_Control_Element.h"

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

#define POS1_NEG127 _Pragma ("GCC warning \"'POS1_NEG127' macro is deprecated, use 'TWOS_COMPLEMENT' instead\"") TWOS_COMPLEMENT
#define ADD_64 _Pragma ("GCC warning \"'ADD_64' macro is deprecated, use 'BINARY_OFFSET' instead\"") BINARY_OFFSET
#define SIGN_BIT _Pragma ("GCC warning \"'SIGN_BIT' macro is deprecated, use 'SIGN_MAGNITUDE' instead\"") SIGN_MAGNITUDE

const uint8_t NORMAL_ENCODER = 4; // A normal rotary encoder sends four pulses per physical 'click'
const uint8_t JOG = 1;            // For jog wheels, you want the highest possible resolution

class RotaryEncoder : public MIDI_Control_Element
{
public:
  RotaryEncoder(uint8_t pinA, uint8_t pinB, uint8_t controllerNumber, uint8_t channel, int speedMultiply = 1, uint8_t pulsesPerStep = NORMAL_ENCODER, relativeCCmode mode = TWOS_COMPLEMENT); // Constructor
  ~RotaryEncoder();                                                                                                                                                                           // Destructor

  static uint8_t twosComplement7bit(int8_t value);                  // Convert an 8-bit two's complement integer to a 7-bit two's complement integer
  static uint8_t twosComplementTo7bitSignedMagnitude(int8_t value); // Convert an 8-bit two's complement integer to 7-bit sign-magnitude format
  static uint8_t mapRelativeCC(int8_t value, relativeCCmode type);  // Convert an 8-bit two's complement integer to a 7-bit value to send over MIDI

private:
  void refresh(); // Check if the encoder position has changed since last time, if so, send the relative movement over MIDI

  uint8_t controllerNumber, channel, pulsesPerStep;
  relativeCCmode mode;
  int speedMultiply;
  long oldVal = 0;
  Encoder enc;
};
#endif