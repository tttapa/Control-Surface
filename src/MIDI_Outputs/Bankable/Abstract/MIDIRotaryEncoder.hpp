#pragma once

#include <AH/STL/utility> // std::forward
#include <Banks/BankableAddresses.hpp>
#include <Def/Def.hpp>
#include <Def/TypeTraits.hpp>
#include <MIDI_Outputs/Abstract/EncoderState.hpp>
#include <MIDI_Outputs/Abstract/MIDIOutputElement.hpp>

#ifdef ARDUINO
#include <Submodules/Encoder/Encoder.h>
#else
#include <Encoder.h> // Mock
#endif

AH_DIAGNOSTIC_WERROR()

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
    GenericMIDIRotaryEncoder(BankAddress bankAddress, Enc &&encoder,
                             int16_t speedMultiply, uint8_t pulsesPerStep,
                             const Sender &sender)
        : address(bankAddress), encoder(std::forward<Enc>(encoder)),
          encstate(speedMultiply, pulsesPerStep), sender(sender) {}

  public:
    void begin() override { begin_if_possible(encoder); }

    void update() override {
        auto encval = encoder.read();
        if (int16_t delta = encstate.update(encval)) {
            sender.send(delta, address.getActiveAddress());
        }
    }

    void setSpeedMultiply(int16_t speedMultiply) {
        encstate.setSpeedMultiply(speedMultiply);
    }
    int16_t getSpeedMultiply() const { return encstate.getSpeedMultiply(); }

  protected:
    BankAddress address;
    Enc encoder;
    EncoderState<decltype(encoder.read())> encstate;

  public:
    Sender sender;
};

template <class BankAddress, class Sender>
using MIDIRotaryEncoder =
    GenericMIDIRotaryEncoder<Encoder, BankAddress, Sender>;

template <class BankAddress, class Sender>
using BorrowedMIDIRotaryEncoder =
    GenericMIDIRotaryEncoder<Encoder &, BankAddress, Sender>;

} // namespace Bankable

END_CS_NAMESPACE

AH_DIAGNOSTIC_POP()
