#pragma once

#include <AH/STL/utility> // std::forward
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

/**
 * @brief   An abstract class for rotary encoders that send MIDI events.
 */
template <class Enc, class Sender>
class GenericMIDIRotaryEncoder : public MIDIOutputElement {
  public:
    /**
     * @brief   Construct a new MIDIRotaryEncoder.
     *
     * @todo    Documentation
     */
    GenericMIDIRotaryEncoder(Enc &&encoder, MIDIAddress address,
                             int16_t speedMultiply, uint8_t pulsesPerStep,
                             const Sender &sender)
        : encoder(std::forward<Enc>(encoder)), address(address),
          encstate(speedMultiply, pulsesPerStep), sender(sender) {}

    void begin() override { begin_if_possible(encoder); }

    void update() override {
        auto encval = encoder.read();
        if (int16_t delta = encstate.update(encval)) {
            sender.send(delta, address);
        }
    }

    void setSpeedMultiply(int16_t speedMultiply) {
        encstate.setSpeedMultiply(speedMultiply);
    }
    int16_t getSpeedMultiply() const { return encstate.getSpeedMultiply(); }

  private:
    Enc encoder;
    MIDIAddress address;
    EncoderState<decltype(encoder.read())> encstate;

  public:
    Sender sender;
};

template <class Sender>
using MIDIRotaryEncoder = GenericMIDIRotaryEncoder<Encoder, Sender>;

template <class Sender>
using BorrowedMIDIRotaryEncoder = GenericMIDIRotaryEncoder<Encoder &, Sender>;

END_CS_NAMESPACE

AH_DIAGNOSTIC_POP()
