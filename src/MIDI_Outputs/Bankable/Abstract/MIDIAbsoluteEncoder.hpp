#pragma once

#if not defined(Encoder_h_) && not defined(IDE)
#error                                                                         \
    "The PJRC Encoder library should be included before the Control-Surface "  \
    "library. (#include <Encoder.h>)"
#endif

#include <Banks/BankableAddresses.hpp>
#include <Def/Def.hpp>
#include <Encoder.h>
#include <MIDI_Outputs/Abstract/MIDIOutputElement.hpp>

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
                               int16_t multiplier, uint8_t pulsesPerStep,
                               const Sender &sender)
        : encoder(std::forward<Enc>(encoder)), address(address),
          multiplier(multiplier), pulsesPerStep(pulsesPerStep), sender(sender) {
    }

    void begin() override {}

    void update() override {
        auto delta = getNewDelta();
        if (delta) {
            address.lock();
            auto oldValue = values[address.getSelection()];
            auto newValue = oldValue + delta;
            newValue = constrain(newValue, 0, maxValue);
            if (oldValue != newValue) {
                sender.send(newValue, address.getActiveAddress());
                values[address.getSelection()] = newValue;
            }
            address.unlock();
        }
    }

    /**
     * @brief   Read the encoder and return the difference (delta) in position
     *          relative to the previous time this method was called.
     */
    long getNewDelta() {
        auto encval = encoder.read();
        long delta = (encval - zeroOffsetPosition) * multiplier / pulsesPerStep;
        if (delta)
            zeroOffsetPosition = encval + delta * pulsesPerStep / multiplier;
        return delta;
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

  private:
    Enc encoder;
    BankAddress address;
    int16_t multiplier;
    uint8_t pulsesPerStep;
    Array<uint16_t, NumBanks> values = {{}};
    long zeroOffsetPosition = 0;

    constexpr static uint16_t maxValue = (1 << Sender::precision()) - 1;

  public:
    Sender sender;
};

template <uint8_t NumBanks, class BankAddress, class Sender>
using MIDIAbsoluteEncoder =
    GenericMIDIAbsoluteEncoder<Encoder, NumBanks, BankAddress, Sender>;

template <uint8_t NumBanks, class BankAddress, class Sender>
using BorrowedMIDIAbsoluteEncoder =
    GenericMIDIAbsoluteEncoder<Encoder &, NumBanks, BankAddress, Sender>;

} // namespace Bankable

END_CS_NAMESPACE