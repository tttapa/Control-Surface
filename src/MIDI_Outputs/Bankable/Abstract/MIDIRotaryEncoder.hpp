#pragma once

#if not defined(Encoder_h_) && not defined(IDE)
#error                                                                         \
    "The PJRC Encoder library should be included before the Control-Surface "  \
    "library. (#include <Encoder.h>)"
#endif

#include <Banks/BankableMIDIOutput.hpp>
#include <Def/Def.hpp>
#include <Encoder.h>
#include <MIDI_Outputs/Abstract/MIDIOutputElement.hpp>

BEGIN_CS_NAMESPACE

namespace Bankable {

/**
 * @brief   An abstract class for rotary encoders that send MIDI events and that
 *          can be added to a Bank.
 */
template <class Sender>
class MIDIRotaryEncoder : public BankableMIDIOutput, public MIDIOutputElement {
  protected:
    /**
     * @brief   Construct a new MIDIRotaryEncoder.
     *
     * @todo    Documentation
     */
    MIDIRotaryEncoder(const OutputBankConfig &config,
                      const EncoderPinList &pins,
                      const MIDICNChannelAddress &address,
                      uint8_t speedMultiply, uint8_t pulsesPerStep,
                      const Sender &sender)
        : BankableMIDIOutput(config), encoder{pins.A, pins.B}, address(address),
          speedMultiply(speedMultiply),
          pulsesPerStep(pulsesPerStep), sender{sender} {}

// For tests only
#ifndef ARDUINO
    MIDIRotaryEncoder(const OutputBankConfig &config, const Encoder &encoder,
                      const MIDICNChannelAddress &address,
                      uint8_t speedMultiply, uint8_t pulsesPerStep,
                      const Sender &sender)
        : BankableMIDIOutput(config), encoder{encoder}, address(address),
          speedMultiply(speedMultiply),
          pulsesPerStep(pulsesPerStep), sender{sender} {}
#endif

  public:
    void begin() final override {}
    void update() final override {
        MIDICNChannelAddress sendAddress = address + getAddressOffset();
        long currentPosition = encoder.read();
        long difference = (currentPosition - previousPosition) / pulsesPerStep;
        // I could do the division inside of the if statement for performance
        if (difference) {
            sender.send(difference * speedMultiply, sendAddress);
            previousPosition += difference * pulsesPerStep;
        }
    }

  private:
    Encoder encoder;
    const MIDICNChannelAddress address;
    const uint8_t speedMultiply;
    const uint8_t pulsesPerStep;
    long previousPosition = 0;

  public:
    Sender sender;
};

} // namespace Bankable

END_CS_NAMESPACE