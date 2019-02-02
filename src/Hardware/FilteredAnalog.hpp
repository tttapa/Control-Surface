/* ✔ */

#pragma once

#include <Def/Def.hpp>
#include <Hardware/ExtendedInputOutput/ExtendedInputOutput.hpp>
#include <Helpers/EMA.hpp>
#include <Helpers/Hysteresis.hpp>
#include <Settings/SettingsWrapper.hpp>

constexpr static uint8_t ADC_BITS = 10;

/**
 * A class that reads and filters an analog input.
 *
 * A map function can be applied to the analog value (e.g. to compensate for
 * logarithmic taper potentiometers or to calibrate the range). The analog input
 * value is filtered using an exponential moving average filter. The settings
 * for this filter can be changed in Settings.hpp.
 */
template <uint8_t PRECISION>
class FilteredAnalog {
  public:
    /**
     * @brief Construct a new FilteredAnalog object.
     *
     * @param analogPin
     *        The analog pin to read from.
     */
    FilteredAnalog(pin_t analogPin); // Constructor

    /**
     * @brief   Specify a mapping function that is applied to the raw
     *          analog value before filtering.
     *
     * @param   fn
     *          A function pointer to the mapping function. This function
     *          should take the filtered value (of `PRECISION` bits wide) as a 
     *          parameter, and should return a value of `PRECISION` bits.
     * 
     * @note    Applying the mapping function before filtering could result in
     *          the noise being amplified to such an extent that filtering it
     *          afterwards would be ineffective.  
     *          That's why the mapping function is applied after filtering and
     *          hysteresis.
     */
    void map(MappingFunction fn);

    /**
     * @brief   Read the analog input value, apply the mapping function, and
     *          update the average.
     *
     * @return  true
     *          The value changed since last time it was updated.
     * @return  false
     *          The value is still the same.
     */
    bool update();

    /**
     * @brief   Get the filtered value of the analog input with the mapping 
     *          function applied.
     *
     * @return  The filtered value of the analog input, as a number
     *          of `PRECISION` bits wide.
     */
    uint8_t getValue() const;

    /**
     * @brief   Get the filtered value of the analog input without the mapping
     *          function applied.
     *
     * @return  The filtered value of the analog input, as a number
     *          of `PRECISION` bits wide.
     */
    uint8_t getRawValue() const;

  private:
    const pin_t analogPin;

    int (*mapFn)(int) = nullptr;

    EMA<ANALOG_FILTER_SHIFT_FACTOR, ANALOG_FILTER_TYPE> filter;
    Hysteresis<ADC_BITS - PRECISION> hysteresis;
};

// ----------------------------- Implementation ----------------------------- //

#include <Control_Surface/Control_Surface_Class.hpp>

using namespace ExtIO;

template <uint8_t PRECISION>
FilteredAnalog<PRECISION>::FilteredAnalog(pin_t analogPin)
    : analogPin(analogPin) {}

template <uint8_t PRECISION>
bool FilteredAnalog<PRECISION>::update() {
    analog_t input =
        ExtIO::analogRead(analogPin); // read the raw analog input value
    input = filter.filter(input);     // apply a low-pass EMA filter
    return hysteresis.update(input);  // apply hysteresis
}

template <uint8_t PRECISION>
uint8_t FilteredAnalog<PRECISION>::getValue() const {
    uint8_t value = getRawValue();
    if (mapFn != nullptr)     // if a map function is specified
        value = mapFn(value); // apply the map function to the value
    return value;
}

template <uint8_t PRECISION>
uint8_t FilteredAnalog<PRECISION>::getRawValue() const {
    return hysteresis.getValue();
}

template <uint8_t PRECISION>
void FilteredAnalog<PRECISION>::map(MappingFunction fn) {
    mapFn = fn;
}