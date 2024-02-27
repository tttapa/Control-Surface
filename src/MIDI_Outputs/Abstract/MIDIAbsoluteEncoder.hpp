#pragma once

#include <AH/STL/type_traits> // std::make_signed
#include <AH/STL/utility>     // std::forward
#include <Def/Def.hpp>
#include <Def/TypeTraits.hpp>
#include <MIDI_Outputs/Abstract/EncoderState.hpp>
#include <MIDI_Outputs/Abstract/MIDIOutputElement.hpp>

#ifdef ARDUINO
#include <Submodules/Encoder/AHEncoder.hpp>
#else
#include <Encoder.h> // Mock
#endif

BEGIN_CS_NAMESPACE

/**
 * @brief   An abstract class for rotary encoders that send absolute MIDI 
 *          events.
 */
template <class Enc, class Sender>
class GenericMIDIAbsoluteEncoder : public MIDIOutputElement {
  public:
    GenericMIDIAbsoluteEncoder(Enc &&encoder, MIDIAddress address,
                               int16_t speedMultiply, uint8_t pulsesPerStep,
                               const Sender &sender)
        : encoder(std::forward<Enc>(encoder)), address(address),
          encstate(speedMultiply, pulsesPerStep), sender(sender) {}

    void begin() override { begin_if_possible(encoder); }

    void update() override {
        auto encval = encoder.read();
        if (int16_t delta = encstate.update(encval)) {
            int16_t oldValue = value;
            int16_t newValue = oldValue + delta;
            newValue = constrain(newValue, 0, maxValue);
            if (oldValue != newValue) {
                value = newValue;
                forcedUpdate();
            }
        }
    }

    /// Send the current value over MIDI, even if the position of the encoder
    /// didn't change.
    void forcedUpdate() { sender.send(value, address); }

    /**
     * @brief   Get the absolute value of the encoder.
     */
    uint16_t getValue() const { return value; }

    /**
     * @brief   Set the absolute value of the encoder.
     */
    void setValue(uint16_t value) { this->value = value; }

    /// Get the maximum possible value that can be returned by @ref getValue.
    static int16_t getMaxValue() { return maxValue; }

    void setSpeedMultiply(int16_t speedMultiply) {
        encstate.setSpeedMultiply(speedMultiply);
    }
    int16_t getSpeedMultiply() const { return encstate.getSpeedMultiply(); }

    /// Get the MIDI address.
    MIDIAddress getAddress() const { return this->address; }
    /// Set the MIDI address.
    void setAddress(MIDIAddress address) { this->address = address; }

    int16_t resetPositionOffset() {
        auto encval = encoder.read();
        return encstate.update(encval);
    }

  private:
    Enc encoder;
    MIDIAddress address;
    int16_t value = 0;
    EncoderState<decltype(encoder.read())> encstate;

    constexpr static int16_t maxValue = uint16_t(1u << Sender::precision()) - 1;

  public:
    Sender sender;
};

template <class Sender>
using MIDIAbsoluteEncoder = GenericMIDIAbsoluteEncoder<AHEncoder, Sender>;

template <class Sender>
using BorrowedMIDIAbsoluteEncoder =
    GenericMIDIAbsoluteEncoder<AHEncoder &, Sender>;

END_CS_NAMESPACE
