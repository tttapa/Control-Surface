#pragma once

#include <Banks/BankableAddresses.hpp>
#include <Def/Def.hpp>
#include <MIDI_Outputs/Abstract/MIDIOutputElement.hpp>

BEGIN_CS_NAMESPACE

namespace Bankable {

/**
 * @brief   An abstract class for rotary encoders that send MIDI events and that
 *          can be added to a Bank.
 */
template <class Enc, class BankAddress, class Sender>
class GenericMIDIRotaryEncoder : public MIDIOutputElement {
  protected:
    /**
     * @brief   Construct a new GenericMIDIRotaryEncoder.
     *
     * @todo    Documentation
     */
    GenericMIDIRotaryEncoder(const BankAddress &bankAddress, Enc &&encoder,
                             uint8_t speedMultiply, uint8_t pulsesPerStep,
                             const Sender &sender)
        : address(bankAddress), encoder(std::forward<Enc>(encoder)),
          speedMultiply(speedMultiply), pulsesPerStep(pulsesPerStep),
          sender(sender) {}

  public:
    void begin() override {}

    void update() override {
        int32_t encval = encoder.read();
        int32_t delta = (encval - deltaOffset) * speedMultiply / pulsesPerStep;
        if (delta) {
            sender.send(delta, address.getActiveAddress());
            deltaOffset += delta * pulsesPerStep / speedMultiply;
        }
    }

  private:
    BankAddress address;
    Enc encoder;
    const uint8_t speedMultiply;
    const uint8_t pulsesPerStep;
    long deltaOffset = 0;

  public:
    Sender sender;
};

#if defined(Encoder_h_) || not defined(ARDUINO)

template <class BankAddress, class Sender>
using MIDIRotaryEncoder =
    GenericMIDIRotaryEncoder<Encoder, BankAddress, Sender>;

template <class BankAddress, class Sender>
using BorrowedMIDIRotaryEncoder =
    GenericMIDIRotaryEncoder<Encoder &, BankAddress, Sender>;

#endif

} // namespace Bankable

END_CS_NAMESPACE