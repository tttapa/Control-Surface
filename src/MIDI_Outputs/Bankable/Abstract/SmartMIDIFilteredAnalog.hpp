#pragma once

#include <AH/Containers/BitArray.hpp>
#include <AH/Hardware/FilteredAnalog.hpp>
#include <AH/STL/algorithm> // std::fill
#include <Banks/BankableAddresses.hpp>
#include <Def/Def.hpp>
#include <MIDI_Outputs/Abstract/MIDIOutputElement.hpp>

BEGIN_CS_NAMESPACE

namespace Bankable {

/**
 * @brief   A class for potentiometers and faders that send MIDI events.
 * 
 * When changing banks, it disables the potentiometer until you move it to the 
 * position where it was last time the current bank was active. This prevents 
 * the values changing when changing banks.
 *
 * The analog input is filtered and hysteresis is applied.
 *
 * @see     FilteredAnalog
 */
template <uint8_t NumBanks, class BankAddress, class Sender>
class SmartMIDIFilteredAnalog : public MIDIOutputElement {
  protected:
    /**
     * @brief   Construct a new SmartMIDIFilteredAnalog.
     *
     * @param   bankAddress
     *          The bankable MIDI address to send to.
     * @param   analogPin
     *          The analog input pin with the wiper of the potentiometer
     *          connected.
     * @param   sender
     *          The MIDI sender to use.
     */
    SmartMIDIFilteredAnalog(BankAddress bankAddress, pin_t analogPin,
                            const Sender &sender)
        : address(bankAddress), filteredAnalog(analogPin), sender(sender) {}

  public:
    /// State of the smart potentiometer.
    enum State {
        /// The potentiometer is active, the position changes will be sent over
        /// MIDI.
        Active,
        /// The value of the potentiometer is lower than the previously recorded
        /// value for the current bank. In order to activate the potentiometer,
        /// you have to move it up.
        Lower,
        /// The value of the potentiometer is higher than the previously
        /// recorded value for the current bank. In order to activate the
        /// potentiometer, you have to move it down.
        Higher,
    };

    void begin() override {
        filteredAnalog.resetToCurrentValue();
        previousBank = address.getSelection();
        std::fill(std::begin(previousValues), std::end(previousValues),
                  initial);
    }

    void update() override {
        auto activeBank = address.getSelection();
        if (filteredAnalog.update() || activeBank != previousBank) {
            auto previousValue = previousValues[activeBank];
            auto value = filteredAnalog.getValue();
            if (activeBank == previousBank && state == Higher) {
                if (value <= previousValue)
                    state = Active;
            } else if (activeBank == previousBank && state == Lower) {
                if (value >= previousValue)
                    state = Active;
            } else if (activeBank != previousBank) {
                if (value == previousValue || previousValue == initial)
                    state = Active;
                else if (value > previousValue)
                    state = Higher;
                else
                    state = Lower;
                previousBank = activeBank;
            }
            if (activeBank == previousBank && state == Active) {
                sender.send(value, address.getActiveAddress());
                previousValues[activeBank] = value;
            }
        }
    }

    /**
     * @brief   Get the state of the smart potentiometer, to know whether the
     *          position has to be lower or higher in order to activate it.
     */
    State getState() const { return state; }

    /**
     * @brief   Activate the potentiometer in the current bank, regardless of
     *          its current and previous position.
     */
    void activate() { state = Active; }

    /**
     * @brief   Specify a mapping function that is applied to the raw
     *          analog value before sending.
     *
     * @param   fn
     *          A function pointer to the mapping function. This function
     *          should take the filtered analog value of @f$ 16 - 
     *          \mathrm{ANALOG\_FILTER\_SHIFT\_FACTOR} @f$ bits as a parameter, 
     *          and should return a value in the same range.
     * 
     * @see     FilteredAnalog::map     
     */
    void map(MappingFunction fn) { filteredAnalog.map(fn); }

    /// Invert the analog value.
    void invert() { filteredAnalog.invert(); }

    /**
     * @brief   Get the raw value of the analog input (this is the value 
     *          without applying the filter or the mapping function first).
     */
    analog_t getRawValue() const { return filteredAnalog.getRawValue(); }

    /**
     * @brief   Get the maximum value that can be returned from @ref getRawValue.
     */
    static constexpr analog_t getMaxRawValue() {
        return FilteredAnalog::getMaxRawValue();
    }

    /**
     * @brief   Get the value of the analog input (this is the value after first
     *          applying the mapping function).
     */
    analog_t getValue() const { return filteredAnalog.getValue(); }

    /**
     * @brief  Get the previous value of the analog input of the given bank.
     */
    analog_t getPreviousValue(setting_t bank) const {
        return previousValues[bank];
    }

    /**
     * @brief  Get the previous value of the analog input of the active bank.
     */
    analog_t getPreviousValue() const {
        return getPreviousValue(address.getSelection());
    }

  protected:
    BankAddress address;
    using FilteredAnalog = AH::FilteredAnalog<Sender::precision()>;
    FilteredAnalog filteredAnalog;
    static_assert(
        Sender::precision() <= 14,
        "Sender precision must be 14 or less, because larger values are "
        "reserved.");
    constexpr static analog_t initial = 1u << 14;
    AH::Array<analog_t, NumBanks> previousValues = {{}};
    State state = Active;
    setting_t previousBank = 0;

  public:
    Sender sender;
};

} // namespace Bankable

END_CS_NAMESPACE
