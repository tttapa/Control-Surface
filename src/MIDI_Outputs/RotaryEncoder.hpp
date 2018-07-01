#ifndef CONTROL_SURFACE_MIDI_OUTPUTS_ROTARYENCODER_HPP_
#define CONTROL_SURFACE_MIDI_OUTPUTS_ROTARYENCODER_HPP_

#ifndef Encoder_h_
#error "PJRC Encoder library required (https://github.com/PaulStoffregen/Encoder)"
#endif

#include "RelativeCCOut.h"

const uint8_t NORMAL_ENCODER = 4; // A normal rotary encoder sends four pulses per physical 'click'
const uint8_t JOG = 1;            // For jog wheels, you want the highest possible resolution

class RotaryEncoder : public RelativeCCOut
{
public:
  RotaryEncoder(uint8_t pinA, uint8_t pinB,
                uint8_t controllerNumber, uint8_t channel,
                int speedMultiply = 1, uint8_t pulsesPerStep = NORMAL_ENCODER,
                relativeCCmode mode = TWOS_COMPLEMENT) // Constructor
      : RelativeCCOut(controllerNumber, channel, mode),
        speedMultiply(speedMultiply), pulsesPerStep(pulsesPerStep),
        enc{pinA, pinB}
  {
  }

private:
  // Check if the encoder position has changed since last time, if so, send the relative movement over MIDI
  void refresh() override
  {
    long currentPosition = enc.read();
    long difference = (currentPosition - previousPosition) / pulsesPerStep;
    send(difference * speedMultiply);
    previousPosition += difference * pulsesPerStep;
  }

  const int speedMultiply;
  const uint8_t pulsesPerStep;

  Encoder enc;
  long previousPosition = 0;
};
#endif // CONTROL_SURFACE_MIDI_OUTPUTS_ROTARYENCODER_HPP_