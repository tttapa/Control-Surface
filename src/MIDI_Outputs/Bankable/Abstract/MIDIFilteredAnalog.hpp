#pragma once

#include <Banks/BankableMIDIOutput.hpp>
#include <Banks/BankableMIDIOutputAddressable.hpp>
#include <Def/Def.hpp>
#include <Hardware/FilteredAnalog.h>
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
                                      public BankableMIDIOutputAddressable {
  protected:
    /**
     * @brief   Construct a new MIDIFilteredAnalogAddressable.
     *
     * @param   analogPin
     *          The analog input pin with the wiper of the potentiometer
     *          connected.
     * @todo    Documentation.
     */
    MIDIFilteredAnalogAddressable(const OutputBankConfigAddressable &config,
                                  pin_t analogPin, uint8_t baseAddress,
                                  uint8_t baseChannel)
        : BankableMIDIOutputAddressable(config), filteredAnalog{analogPin},
          baseAddress(baseAddress), baseChannel(baseChannel) {}

  public:
    void begin() final override {}
    void update() final override {
        if (filteredAnalog.update())
            send(filteredAnalog.getValue(), getChannel(baseChannel),
                 getAddress(baseAddress));
    }

    /**
     * @brief   Specify a mapping function that is applied to the raw
     *          analog value before sending.
     *
     * @param   fn
     *          A function pointer to the mapping function. This function
     *          should take the 10-bit analog value as a parameter, and
     *          should return a 10-bit value.
     */
    void map(MappingFunction fn) { filteredAnalog.map(fn); }

  private:
    FilteredAnalog<PRECISION> filteredAnalog;
    const uint8_t baseAddress;
    const uint8_t baseChannel;
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
    MIDIFilteredAnalog(const OutputBank &bank, pin_t analogPin, uint8_t channel)
        : BankableMIDIOutput(bank), filteredAnalog{analogPin},
          channel(channel) {}

  public:
    void begin() final override {}
    void update() final override {
        if (filteredAnalog.update())
            send(filteredAnalog.getValue(), getChannel(channel));
    }

    /**
     * @brief   Specify a mapping function that is applied to the raw
     *          analog value before sending.
     *
     * @param   fn
     *          A function pointer to the mapping function. This function
     *          should take the 10-bit analog value as a parameter, and
     *          should return a 10-bit value.
     */
    void map(MappingFunction fn) { filteredAnalog.map(fn); }

  private:
    FilteredAnalog<PRECISION> filteredAnalog;
    const uint8_t channel;
};

} // namespace Bankable