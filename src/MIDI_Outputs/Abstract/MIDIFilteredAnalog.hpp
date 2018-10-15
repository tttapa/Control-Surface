#pragma once

#include <Def/Def.hpp>
#include <Hardware/FilteredAnalog.hpp>
#include <MIDI_Outputs/Abstract/MIDIOutputElement.hpp>

/**
 * @brief   A class for potentiometers and faders that send MIDI events.
 *
 * The analog input is filtered and hysteresis is applied.
 *
 * @tparam  PRECISION
 *          The analog precision in bits.
 *
 * @see     FilteredAnalog
 */
template <ContinuousSendFunction7Bit send, uint8_t PRECISION>
class MIDIFilteredAnalogAddressable : public MIDIOutputElement {
  protected:
    /**
     * @brief   Construct a new MIDIFilteredAnalog.
     *
     * @param   analogPin
     *          The analog input pin with the wiper of the potentiometer
     *          connected.
     * @todo    Documentation.
     */
    MIDIFilteredAnalogAddressable(pin_t analogPin,
                                  const MIDICNChannelAddress &address)
        : filteredAnalog{analogPin}, address{address} {}

  public:
    void begin() final override {}
    void update() final override {
        if (filteredAnalog.update())
            send(filteredAnalog.getValue(), address);
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
    const MIDICNChannelAddress address;
};

// -------------------------------------------------------------------------- //

/**
 * @brief   A class for potentiometers and faders that send MIDI events (with
 *          only a channel, no address).
 *
 * The analog input is filtered and hysteresis is applied.
 *
 * @tparam  PRECISION
 *          The analog precision in bits.
 *
 * @see     FilteredAnalog
 */
template <ContinuousSendFunction14Bit send, uint8_t PRECISION>
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
    MIDIFilteredAnalog(pin_t analogPin, const MIDICNChannel &address)
        : filteredAnalog{analogPin}, address(address) {}

  public:
    void begin() final override {}
    void update() final override {
        if (filteredAnalog.update())
            send(filteredAnalog.getValue(), address);
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
    const MIDICNChannelAddress address;
};