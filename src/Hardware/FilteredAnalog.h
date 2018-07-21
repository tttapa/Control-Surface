#pragma once

#include "../Hardware/ExtendedInputOutput/ExtendedInputOutput.h"
#include "../Helpers/EMA.h"
#include "../Helpers/Hysteresis.h"
#include "../Settings/SettingsWrapper.h"

constexpr static uint8_t ADC_BITS = 10;

using MappingFunction = int(*(int));

/**
 * A class that reads and filters an analog input.
 *
 * A map function can be applied to the analog value (e.g. to compensate for
 * logarithmic taper potentiometers or to calibrate the range). The analog input
 * value is filtered using an exponential moving average filter. The settings
 * for this filter can be changed in Settings.h.
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
     *          should take the 10-bit analog value as a parameter, and
     *          should return a 10-bit value.
     */
    void map(MappingFunction fn);

    /**
     * @brief Read the analog input value, apply the mapping function, and
     * update the average.
     *
     * @return  true
     *          The value changed since last time it was updated.
     * @return  false
     *          Otherwise.
     */
    bool update();

    /**
     * @brief   Get the filtered value of the analog input.
     *
     * @return  The filtered value of the analog input, as a number
     *          of `PRECISION` bits wide.
     * @return  0xFFFF if the update() has not been called yet.
     */
    uint16_t getValue();

  private:
    const pin_t analogPin;

    int (*mapFn)(int) = nullptr;

    EMA<ANALOG_FILTER_SHIFT_FACTOR, ANALOG_FILTER_TYPE> filter;
    Hysteresis<ADC_BITS - PRECISION> hysteresis;

    uint16_t value = 0xFFFF;
};

// ------------------------ Implementations ------------------------ //

#include "../Control_Surface/Control_Surface_Class.h"

using namespace ExtIO;

template <uint8_t PRECISION>
FilteredAnalog<PRECISION>::FilteredAnalog(pin_t analogPin)
    : analogPin(analogPin) {} // Constructor

template <uint8_t PRECISION>
bool FilteredAnalog<PRECISION>::update() {
    unsigned int input =
        ExtIO::analogRead(analogPin); // read the raw analog input value
    if (mapFn != nullptr)             // if a map function is specified
        input = mapFn(input);         // apply the map function to the value
    input = filter.filter(input);     // apply a low-pass EMA filter
    uint16_t newValue =
        hysteresis.getOutputLevel(input); // map from the 10-bit analog input
                                          // value to a value of `PRECISION`
                                          // bits wide, applying hysteresis
    bool changed = newValue != value;
    value = newValue;
    return changed;
}

template <uint8_t PRECISION>
uint16_t FilteredAnalog<PRECISION>::getValue() {
    return value;
}

template <uint8_t PRECISION>
void FilteredAnalog<PRECISION>::map(MappingFunction fn) {
    mapFn = fn;
}