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
    MIDIFilteredAnalogAddressable(pin_t analogPin,
                                  const MIDICNChannelAddress &address,
                                  const Sender &sender)
        : filteredAnalog{analogPin}, address{address}, sender{sender} {}

  public:
    void begin() final override {}
    void update() final override {
        if (filteredAnalog.update())
            sender.send(filteredAnalog.getValue(), address);
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
    uint8_t getRawValue() const { return filteredAnalog.getRawValue(); }

    /**
     * @brief   Get the value of the analog input (this is the value after first
     *          applying the mapping function).
     */
    uint8_t getValue() const { return filteredAnalog.getValue(); }

  private:
    FilteredAnalog<PRECISION> filteredAnalog;
    const MIDICNChannelAddress address;
    Sender sender;
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
    MIDIFilteredAnalog(pin_t analogPin, const MIDICNChannel &address,
                       const Sender &sender)
        : filteredAnalog{analogPin}, address(address), sender{sender} {}

  public:
    void begin() final override {}
    void update() final override {
        if (filteredAnalog.update())
            sender.send(filteredAnalog.getValue(), address);
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
    FilteredAnalog<PRECISION, 6, ANALOG_FILTER_SHIFT_FACTOR, uint32_t>
        filteredAnalog;
    const MIDICNChannelAddress address;
    Sender sender;
};