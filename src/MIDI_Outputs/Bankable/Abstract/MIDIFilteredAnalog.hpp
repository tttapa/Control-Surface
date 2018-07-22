#pragma once

#include <Banks/BankableMIDIOutputAddressable.hpp>
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
template <class Sender, uint8_t PRECISION>
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
    MIDIFilteredAnalogAddressable(pin_t analogPin, uint8_t baseAddress,
                                  uint8_t baseChannel)
        : filteredAnalog{analogPin}, baseAddress(baseAddress),
          baseChannel(baseChannel) {}

  public:
    void update() final override {
        if (filteredAnalog.update())
            Sender::send(filteredAnalog.getValue(), getChannel(baseChannel),
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
template <class Sender, uint8_t PRECISION>
class MIDIFilteredAnalog : public MIDIOutputElement {
  protected:
    /**
     * @brief   Construct a new MIDIFilteredAnalog.
     *
     * @param   analogPin
     *          The analog input pin with the wiper of the potentiometer
     *          connected.
     * @todo    Documentation.
     */
    MIDIFilteredAnalog(pin_t analogPin, uint8_t channel)
        : filteredAnalog{analogPin}, channel(channel) {}

  public:
    void update() final override {
        if (filteredAnalog.update())
            Sender::send(filteredAnalog.getValue(), channel);
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