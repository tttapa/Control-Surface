#pragma once

#include <Def/Def.hpp>
#include <Hardware/ExtendedInputOutput/ExtendedInputOutput.hpp>
#include <Helpers/EMA.hpp>
#include <Helpers/Hysteresis.hpp>
#include <Helpers/IncreaseBitDepth.hpp>
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
template <uint8_t PRECISION, uint8_t UPSAMPLE = 0,
          uint8_t FILTER_SHIFT_FACTOR = ANALOG_FILTER_SHIFT_FACTOR,
          class FILTER_TYPE = ANALOG_FILTER_TYPE>
class FilteredAnalog {
  public:
    /**
     * @brief   Construct a new FilteredAnalog object.
     *
     * @param   analogPin
     *          The analog pin to read from.
     */
    FilteredAnalog(pin_t analogPin) : analogPin(analogPin) {}

    /**
     * @brief   Specify a mapping function that is applied to the raw
     *          analog value before filtering.
     *
     * @param   fn
     *          A function pointer to the mapping function. This function
     *          should take the filtered value (of 10 + UPSAMPLE bits wide) as a 
     *          parameter, and should return a value of 10 bits + UPSAMPLE .
     * 
     * @note    Applying the mapping function before filtering could result in
     *          the noise being amplified to such an extent that filtering it
     *          afterwards would be ineffective.  
     *          Applying it after hysteresis would result in a lower resolution.  
     *          That's why the mapping function is applied after filtering and
     *          before hysteresis.
     */
    void map(MappingFunction fn) { mapFn = fn; }

    /**
     * @brief   Read the analog input value, apply the mapping function, and
     *          update the average.
     *
     * @return  true
     *          The value changed since last time it was updated.
     * @return  false
     *          The value is still the same.
     */
    bool update() {
        analog_t input = getRawValue();  // read the raw analog input value
        input = filter.filter(input);    // apply a low-pass EMA filter
        if (mapFn)                       // If a mapping function is specified,
            input = mapFn(input);        // apply it
        return hysteresis.update(input); // apply hysteresis, and return true if
        // the value changed since last time
    }

    /**
     * @brief   Get the filtered value of the analog input with the mapping 
     *          function applied.
     *
     * @return  The filtered value of the analog input, as a number
     *          of `PRECISION` bits wide.
     */
    analog_t getValue() const { return hysteresis.getValue(); }

    /**
     * @brief   Get the filtered value of the analog input with the mapping 
     *          function applied as a floating point number from 0.0 to 1.0.
     * 
     * @return  The filtered value of the analog input, as a number
     *          from 0.0 to 1.0.
     */
    float getFloatValue() const {
        return getValue() * (1.0f / (ldexpf(1.0f, PRECISION) - 1.0f));
    }

    /**
     * @brief   Read the raw value of the analog input any filtering or mapping
     *          applied, but with its bit depth increased by @c UPSAMPLE.
     */
    analog_t getRawValue() const {
        return increaseBitDepth<ADC_BITS + UPSAMPLE, ADC_BITS, analog_t,
                                analog_t>(ExtIO::analogRead(analogPin));
    }

  private:
    const pin_t analogPin;

    MappingFunction mapFn = nullptr;

    static_assert(
        ADC_BITS + UPSAMPLE + FILTER_SHIFT_FACTOR <=
            sizeof(FILTER_TYPE) * CHAR_BIT,
        "Error: FILTER_TYPE is not wide enough to hold the maximum value");
    static_assert(
        ADC_BITS + UPSAMPLE <= sizeof(analog_t) * CHAR_BIT,
        "Error: analog_t is not wide enough to hold the maximum value");

    EMA<FILTER_SHIFT_FACTOR, FILTER_TYPE> filter;
    Hysteresis<ADC_BITS + UPSAMPLE - PRECISION, analog_t, analog_t> hysteresis;
};
