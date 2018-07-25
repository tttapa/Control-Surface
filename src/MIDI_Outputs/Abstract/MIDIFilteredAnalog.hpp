#pragma once

#include <Hardware/FilteredAnalog.h>
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
template <class Sender, uint8_t PRECISION>
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
    MIDIFilteredAnalogAddressable(pin_t analogPin, uint8_t address,
                                  uint8_t channel)
        : filteredAnalog{analogPin}, address(address), channel(channel) {}

  public:
    void begin() final override {}
    void update() final override {
        if (filteredAnalog.update())
            Sender::send(filteredAnalog.getValue(), channel, address);
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
    const uint8_t address;
    const uint8_t channel;
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
    void begin() final override {}
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