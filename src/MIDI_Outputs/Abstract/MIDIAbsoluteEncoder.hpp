#pragma once

#if not defined(Encoder_h_) && not defined(IDE)
#error                                                                         \
    "The PJRC Encoder library should be included before the Control-Surface "  \
    "library. (#include <Encoder.h>)"
#endif

#include <Def/Def.hpp>
#include <Encoder.h>
#include <MIDI_Outputs/Abstract/MIDIOutputElement.hpp>

BEGIN_CS_NAMESPACE

/**
 * @brief   An abstract class for rotary encoders that send absolute MIDI 
 *          events.
 */
template <class Sender>
class MIDIAbsoluteEncoder : public MIDIOutputElement {
  protected:
        MIDIAbsoluteEncoder(const EncoderPinList &pins,
                            const MIDICNChannelAddress &address,
                            int16_t multiplier, uint8_t pulsesPerStep,
                            const Sender &sender)
        : encoder{pins.A, pins.B}, address(address),
          multiplier(multiplier),
          pulsesPerStep(pulsesPerStep), sender(sender) {}
          
  public:
    void begin() override {}
    void update() override {
        long currentPosition = getValue();
        if (currentPosition != previousPosition) {
            sender.send(currentPosition, address);
            previousPosition = currentPosition;
        }
    }

    analog_t getValue() { 
      auto maxval = (1 << Sender::precision()) - 1;
      noInterrupts();
      auto rawval = encoder.read() * multiplier / pulsesPerStep;
      noInterrupts(); // encoder.read() enables interrupts :(
      auto val = constrain(rawval, 0, maxval);
      if (val != rawval)
        encoder.write(val * pulsesPerStep / multiplier);
      interrupts();
      return val;
    }

  private:
    Encoder encoder;
    const MIDICNChannelAddress address;
    const int16_t multiplier;
    const uint8_t pulsesPerStep;
    long previousPosition = 0;

  public:
    Sender sender;
};

END_CS_NAMESPACE