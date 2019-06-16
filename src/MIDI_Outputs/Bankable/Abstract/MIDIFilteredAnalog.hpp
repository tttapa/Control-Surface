#pragma once

#include <Banks/BankableMIDIOutput.hpp>
#include <Def/Def.hpp>
#include <Hardware/FilteredAnalog.hpp>
#include <MIDI_Outputs/Abstract/MIDIOutputElement.hpp>

namespace Bankable {

/**
 * @brief   A class for potentiometers and faders that send MIDI events and that
 *          can be added to a Bank.
 *
 * The analog input is filtered and hysteresis is applied.
 *
 * @tparam  PRECISION
 *          The analog precision in bits.
 *
 * @see     FilteredAnalog
 */
template <ContinuousSendFunction7Bit send, uint8_t PRECISION>
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
                                  const MIDICNChannelAddress &baseAddress)
        : BankableMIDIOutput{config}, filteredAnalog{analogPin},
          baseAddress{baseAddress} {}

  public:
    void begin() final override {}
    void update() final override {
        if (filteredAnalog.update())
            send(filteredAnalog.getValue(), baseAddress + getAddressOffset());
    }

    /**
     * @brief   Specify a mapping function that is applied to the raw
     *          analog value before sending.
     *
     * @param   fn
     *          A function pointer to the mapping function. This function
     *          should take the filtered analog value of `PRECISION` bits as a 
     *          parameter, and should return a value of `PRECISION` bits.
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
    FilteredAnalog<PRECISION> filteredAnalog;
    const MIDICNChannelAddress baseAddress;
};

// -------------------------------------------------------------------------- //

/**
 * @brief   A class for potentiometers and faders that send MIDI events (with
 *          only a channel, no address) and that can be added to a Bank.
 *
 * The analog input is filtered and hysteresis is applied.
 *
 * @tparam  PRECISION
 *          The analog precision in bits.
 *
 * @see     FilteredAnalog
 */
template <ContinuousSendFunction14Bit send, uint8_t PRECISION>
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
                       const MIDICNChannel &address)
        : BankableMIDIOutput{bank}, filteredAnalog{analogPin}, address{
                                                                   address} {}

  public:
    void begin() final override {}
    void update() final override {
        if (filteredAnalog.update())
            send(filteredAnalog.getValue(), address + getAddressOffset());
    }

    /**
     * @brief   Specify a mapping function that is applied to the raw
     *          analog value before sending.
     *
     * @param   fn
     *          A function pointer to the mapping function. This function
     *          should take the filtered analog value of `PRECISION` bits as a 
     *          parameter, and should return a value of `PRECISION` bits.
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
    FilteredAnalog<PRECISION> filteredAnalog;
    const MIDICNChannelAddress address;
};

} // namespace Bankable