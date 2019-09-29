#pragma once

#include <Banks/BankableMIDIOutput.hpp>
#include <Def/Def.hpp>
#include <Hardware/FilteredAnalog.hpp>
#include <MIDI_Outputs/Abstract/MIDIOutputElement.hpp>

BEGIN_CS_NAMESPACE

namespace Bankable {

/**
 * @brief   A class for potentiometers and faders that send MIDI events and that
 *          can be added to a Bank.
 *
 * The analog input is filtered and hysteresis is applied.
 *
 * @see     FilteredAnalog
 */
template <class Sender>
class MIDIFilteredAnalogAddressable : public MIDIOutputElement,
                                      public BankableMIDIOutput {
  protected:
    /**
     * @brief   Construct a new MIDIFilteredAnalog.
     *
     * @param   analogPin
     *          The analog input pin with the wiper of the potentiometer
     *          connected.
     * @todo    Documentation.
     */
    MIDIFilteredAnalogAddressable(const OutputBankConfig &config,
                                  pin_t analogPin,
                                  const MIDICNChannelAddress &baseAddress,
                                  const Sender &sender)
        : BankableMIDIOutput{config}, filteredAnalog{analogPin},
          baseAddress{baseAddress}, sender{sender} {}

  public:
    void begin() final override {}
    void update() final override {
        if (filteredAnalog.update())
            sender.send(filteredAnalog.getValue(),
                        baseAddress + getAddressOffset());
    }

    /**
     * @brief   Specify a mapping function that is applied to the raw
     *          analog value before sending.
     *
     * @param   fn
     *          A function pointer to the mapping function. This function
     *          should take the filtered analog value of 16 bits as a 
     *          parameter, and should return a value of 16 bits.
     * 
     * @see     FilteredAnalog::map     
     */
    void map(MappingFunction fn) { filteredAnalog.map(fn); }

    /**
     * @brief   Get the raw value of the analog input (this is the value 
     *          without applying the mapping function first).
     */
    uint8_t getRawValue() const { return filteredAnalog.getRawValue(); }

    /**
     * @brief   Get the value of the analog input (this is the value after first
     *          applying the mapping function).
     */
    uint8_t getValue() const { return filteredAnalog.getValue(); }

  private:
    FilteredAnalog<Sender::precision(), 16 - ADC_BITS,
                   ANALOG_FILTER_SHIFT_FACTOR, uint32_t>
        filteredAnalog;
    const MIDICNChannelAddress baseAddress;
    Sender sender;
};

// -------------------------------------------------------------------------- //

/**
 * @brief   A class for potentiometers and faders that send MIDI events (with
 *          only a channel, no address) and that can be added to a Bank.
 *
 * The analog input is filtered and hysteresis is applied.
 *
 * @see     FilteredAnalog
 */
template <class Sender>
class MIDIFilteredAnalog : public MIDIOutputElement, public BankableMIDIOutput {
  protected:
    /**
     * @brief   Construct a new MIDIFilteredAnalog.
     *
     * @param   analogPin
     *          The analog input pin with the wiper of the potentiometer
     *          connected.
     * @todo    Bank Config
     */
    MIDIFilteredAnalog(const OutputBankConfig &bank, pin_t analogPin,
                       const MIDICNChannel &address, const Sender &sender)
        : BankableMIDIOutput{bank},
          filteredAnalog{analogPin}, address{address}, sender{sender} {}

  public:
    void begin() final override {}
    void update() final override {
        if (filteredAnalog.update())
            sender.send(filteredAnalog.getValue(),
                        address + getAddressOffset());
    }

    /**
     * @brief   Specify a mapping function that is applied to the raw
     *          analog value before sending.
     *
     * @param   fn
     *          A function pointer to the mapping function. This function
     *          should take the filtered analog value of 10 bits as a 
     *          parameter, and should return a value of 10 bits.
     * 
     * @see     FilteredAnalog::map     
     */
    void map(MappingFunction fn) { filteredAnalog.map(fn); }

    /**
     * @brief   Get the raw value of the analog input (this is the value 
     *          without applying the mapping function first).
     */
    analog_t getRawValue() const { return filteredAnalog.getRawValue(); }

    /**
     * @brief   Get the value of the analog input (this is the value after first
     *          applying the mapping function).
     */
    analog_t getValue() const { return filteredAnalog.getValue(); }

  private:
    FilteredAnalog<Sender::precision(), 16 - ADC_BITS,
                   ANALOG_FILTER_SHIFT_FACTOR, uint32_t>
        filteredAnalog;
    const MIDICNChannelAddress address;
    Sender sender;
};

} // namespace Bankable

END_CS_NAMESPACE