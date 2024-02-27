#pragma once

#include <AH/STL/utility> // std::forward
#include <Banks/BankableAddresses.hpp>
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

namespace Bankable {

/**
 * @brief   An abstract class for rotary encoders that send absolute MIDI 
 *          events.
 */
template <class Enc, uint8_t NumBanks, class BankAddress, class Sender>
class GenericMIDIAbsoluteEncoder : public MIDIOutputElement {
  public:
    GenericMIDIAbsoluteEncoder(const BankAddress &address, Enc &&encoder,
                               int16_t speedMultiply, uint8_t pulsesPerStep,
                               const Sender &sender)
        : encoder(std::forward<Enc>(encoder)), address(address),
          encstate(speedMultiply, pulsesPerStep), sender(sender) {}

    void begin() override { begin_if_possible(encoder); }

    void update() override {
        auto encval = encoder.read();
        if (int16_t delta = encstate.update(encval)) {
            address.lock();
            int16_t oldValue = values[address.getSelection()];
            int16_t newValue = oldValue + delta;
            newValue = constrain(newValue, 0, maxValue);
            if (oldValue != newValue) {
                values[address.getSelection()] = newValue;
                forcedUpdate();
            }
            address.unlock();
        }
    }

    /// Send the current value over MIDI, even if the position of the encoder
    /// didn't change.
    void forcedUpdate() {
        sender.send(values[address.getSelection()], address.getActiveAddress());
    }
    void forcedUpdate(setting_t bank) {
        sender.send(values[bank], address.getActiveAddress(bank));
    }

    /**
     * @brief   Get the absolute value of the encoder in the given bank.
     */
    uint16_t getValue(setting_t bank) const { return values[bank]; }
    /**
     * @brief   Get the absolute value of the encoder in the active bank.
     */
    uint16_t getValue() const { return getValue(address.getSelection()); }

    /**
     * @brief   Set the absolute value of the encoder in the given bank.
     */
    void setValue(uint16_t value, setting_t bank) { values[bank] = value; }
    /**
     * @brief   Set the absolute value of the encoder in the active bank.
     */
    void setValue(uint16_t value) { setValue(value, address.getSelection()); }

    /// Get the maximum possible value that can be returned by @ref getValue.
    static int16_t getMaxValue() { return maxValue; }

    void setSpeedMultiply(int16_t speedMultiply) {
        encstate.setSpeedMultiply(speedMultiply);
    }
    int16_t getSpeedMultiply() const { return encstate.getSpeedMultiply(); }

    int16_t resetPositionOffset() {
        auto encval = encoder.read();
        return encstate.update(encval);
    }

  protected:
    Enc encoder;
    BankAddress address;
    Array<int16_t, NumBanks> values = {{}};
    EncoderState<decltype(encoder.read())> encstate;

    constexpr static int16_t maxValue = uint16_t(1u << Sender::precision()) - 1;

  public:
    Sender sender;
};

template <uint8_t NumBanks, class BankAddress, class Sender>
using MIDIAbsoluteEncoder =
    GenericMIDIAbsoluteEncoder<AHEncoder, NumBanks, BankAddress, Sender>;

template <uint8_t NumBanks, class BankAddress, class Sender>
using BorrowedMIDIAbsoluteEncoder =
    GenericMIDIAbsoluteEncoder<AHEncoder &, NumBanks, BankAddress, Sender>;

} // namespace Bankable

END_CS_NAMESPACE
