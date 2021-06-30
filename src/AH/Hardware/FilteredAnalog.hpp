#pragma once

#include <AH/Settings/Warnings.hpp>
AH_DIAGNOSTIC_WERROR() // Enable errors on warnings

#include <AH/Filters/EMA.hpp>
#include <AH/Filters/Hysteresis.hpp>
#include <AH/Hardware/ExtendedInputOutput/ExtendedInputOutput.hpp>
#include <AH/Hardware/Hardware-Types.hpp>
#include <AH/Math/IncreaseBitDepth.hpp>
#include <AH/Math/MinMaxFix.hpp>
#include <AH/STL/type_traits> // std::enable_if, std::is_constructible
#include <AH/STL/utility> // std::forward
#include <AH/Settings/SettingsWrapper.hpp>

BEGIN_AH_NAMESPACE

/**
 * @brief   Helper to determine how many of the remaining bits of the filter 
 *          data types can be used to achieve higher precision.
 */
template <uint8_t FilterShiftFactor, class FilterType, class AnalogType>
struct MaximumFilteredAnalogIncRes {
    constexpr static uint8_t value =
        min(sizeof(FilterType) * CHAR_BIT - ADC_BITS - FilterShiftFactor,
            sizeof(AnalogType) * CHAR_BIT - ADC_BITS);
};

/**
 * @brief   FilteredAnalog base class with generic MappingFunction.
 * 
 * @see FilteredAnalog
 */
template <class MappingFunction, uint8_t Precision = 10,
          uint8_t FilterShiftFactor = ANALOG_FILTER_SHIFT_FACTOR,
          class FilterType = ANALOG_FILTER_TYPE, class AnalogType = analog_t,
          uint8_t IncRes = MaximumFilteredAnalogIncRes<
              FilterShiftFactor, FilterType, AnalogType>::value>
class GenericFilteredAnalog {
  public:
    /**
     * @brief   Construct a new GenericFilteredAnalog object.
     *
     * @param   analogPin
     *          The analog pin to read from.
     * @param   mapFn
     *          The mapping function
     * @param   initial
     *          The initial value of the filter.
     */
    GenericFilteredAnalog(pin_t analogPin, MappingFunction mapFn,
                          AnalogType initial = 0)
        : analogPin(analogPin), mapFn(std::forward<MappingFunction>(mapFn)),
          filter(increaseBitDepth<ADC_BITS + IncRes, Precision, AnalogType,
                                  AnalogType>(initial)) {}

    /**
     * @brief   Reset the filter to the given value.
     * 
     * @param   value 
     *          The value to reset the filter state to.
     * 
     * @todo    Should the filter be initialized to the first value that is read
     *          instead of to zero? This would require adding a `begin` method.
     */
    void reset(AnalogType value = 0) {
        AnalogType widevalue = increaseBitDepth<ADC_BITS + IncRes, Precision,
                                                AnalogType, AnalogType>(value);
        filter.reset(widevalue);
        hysteresis.setValue(widevalue);
    }

    /**
     * @brief   Reset the filtered value to the value that's currently being
     *          measured at the analog input.
     * 
     * This is useful to avoid transient effects upon initialization.
     */
    void resetToCurrentValue() {
        AnalogType widevalue = getRawValue();
        filter.reset(widevalue);
        hysteresis.setValue(widevalue);
    }

    /**
     * @brief   Specify a mapping function/functor that is applied to the analog
     *          value after filtering and before applying hysteresis.
     *
     * @param   fn
     *          This functor should have a call operator that takes the filtered
     *          value (of ADC_BITS + IncRes bits wide) as a parameter, 
     *          and returns a value of ADC_BITS + IncRes bits wide.
     * 
     * @note    Applying the mapping function before filtering could result in
     *          the noise being amplified to such an extent that filtering it
     *          afterwards would be ineffective.  
     *          Applying it after hysteresis would result in a lower resolution.  
     *          That's why the mapping function is applied after filtering and
     *          before hysteresis.
     */
    void map(MappingFunction fn) { mapFn = std::forward<MappingFunction>(fn); }

    /**
     * @brief   Get a reference to the mapping function.
     */
    MappingFunction &getMappingFunction() { return mapFn; }
    /**
     * @brief   Get a reference to the mapping function.
     */
    const MappingFunction &getMappingFunction() const { return mapFn; }

    /**
     * @brief   Read the analog input value, apply the mapping function, and
     *          update the average.
     *
     * @retval  true
     *          The value changed since last time it was updated.
     * @retval  false
     *          The value is still the same.
     */
    bool update() {
        AnalogType input = getRawValue(); // read the raw analog input value
        input = filter.filter(input);     // apply a low-pass EMA filter
        input = mapFnHelper(input);       // apply the mapping function
        return hysteresis.update(input);  // apply hysteresis, and return true
        // if the value changed since last time
    }

    /**
     * @brief   Get the filtered value of the analog input (with the mapping 
     *          function applied).
     * 
     * @note    This function just returns the value from the last call to
     *          @ref update, it doesn't read the analog input again.
     *
     * @return  The filtered value of the analog input, as a number
     *          of `Precision` bits wide.
     */
    AnalogType getValue() const { return hysteresis.getValue(); }

    /**
     * @brief   Get the filtered value of the analog input with the mapping 
     *          function applied as a floating point number from 0.0 to 1.0.
     * 
     * @return  The filtered value of the analog input, as a number
     *          from 0.0 to 1.0.
     */
    float getFloatValue() const {
        return getValue() * (1.0f / (ldexpf(1.0f, Precision) - 1.0f));
    }

    /**
     * @brief   Read the raw value of the analog input without any filtering or
     *          mapping applied, but with its bit depth increased by @c IncRes.
     */
    AnalogType getRawValue() const {
        AnalogType value = ExtIO::analogRead(analogPin);
#ifdef ESP8266
        if (value > 1023)
            value = 1023;
#endif
        return increaseBitDepth<ADC_BITS + IncRes, ADC_BITS, AnalogType>(value);
    }

    /**
     * @brief   Get the maximum value that can be returned from @ref getRawValue.
     */
    constexpr static AnalogType getMaxRawValue() {
        return (1ul << (ADC_BITS + IncRes)) - 1ul;
    }

    /**
     * @brief   Select the configured ADC resolution. By default, it is set to
     *          the maximum resolution supported by the hardware.
     * 
     * @see     @ref ADC_BITS "ADC_BITS"
     * @see     @ref ADCConfig.hpp "ADCConfig.hpp"
     */
    static void setupADC() {
#if HAS_ANALOG_READ_RESOLUTION
        analogReadResolution(ADC_BITS);
#endif
    }

  private:
    /// Helper function that applies the mapping function if it's enabled.
    /// This function is only enabled if MappingFunction is explicitly
    /// convertible to bool.
    template <typename M = MappingFunction>
    typename std::enable_if<std::is_constructible<bool, M>::value,
                            AnalogType>::type
    mapFnHelper(AnalogType input) {
        return bool(mapFn) ? mapFn(input) : input;
    }

    /// Helper function that applies the mapping function without checking if
    /// it's enabled.
    /// This function is only enabled if MappingFunction is not convertible to
    /// bool.
    template <typename M = MappingFunction>
    typename std::enable_if<!std::is_constructible<bool, M>::value,
                            AnalogType>::type
    mapFnHelper(AnalogType input) {
        return mapFn(input);
    }

  private:
    pin_t analogPin;
    MappingFunction mapFn;

    using EMA_t = EMA<FilterShiftFactor, AnalogType, FilterType>;

    static_assert(
        ADC_BITS + IncRes + FilterShiftFactor <= sizeof(FilterType) * CHAR_BIT,
        "Error: FilterType is not wide enough to hold the maximum value");
    static_assert(
        ADC_BITS + IncRes <= sizeof(AnalogType) * CHAR_BIT,
        "Error: AnalogType is not wide enough to hold the maximum value");
    static_assert(
        Precision <= ADC_BITS + IncRes,
        "Error: Precision is larger than the increased ADC precision");
    static_assert(EMA_t::supports_range(AnalogType(0), getMaxRawValue()),
                  "Error: EMA filter type doesn't support full ADC range");

    EMA_t filter;
    Hysteresis<ADC_BITS + IncRes - Precision, AnalogType, AnalogType>
        hysteresis;
};

/**
 * @brief   A class that reads and filters an analog input.
 *
 * A map function can be applied to the analog value (e.g. to compensate for
 * logarithmic taper potentiometers or to calibrate the range). The analog input
 * value is filtered using an exponential moving average filter. The default
 * settings for this filter can be changed in Settings.hpp.  
 * After filtering, hysteresis is applied to prevent flipping back and forth 
 * between two values when the input is not changing.
 * 
 * @tparam  Precision
 *          The number of bits of precision the output should have.
 * @tparam  FilterShiftFactor
 *          The number of bits used for the EMA filter.
 *          The pole location is
 *          @f$ 1 - \left(\frac{1}{2}\right)^{\text{FilterShiftFactor}} @f$.  
 *          A lower shift factor means less filtering (@f$0@f$ is no filtering),
 *          and a higher shift factor means more filtering (and more latency).
 * @tparam  FilterType
 *          The type to use for the intermediate types of the filter.  
 *          Should be at least 
 *          @f$ \text{ADC_BITS} + \text{IncRes} + 
 *          \text{FilterShiftFactor} @f$ bits wide.
 * @tparam  AnalogType
 *          The type to use for the analog values.  
 *          Should be at least @f$ \text{ADC_BITS} + \text{IncRes} @f$ 
 *          bits wide.
 * @tparam  IncRes
 *          The number of bits to increase the resolution of the analog reading
 *          by.
 * 
 * @ingroup AH_HardwareUtils
 */
template <uint8_t Precision = 10,
          uint8_t FilterShiftFactor = ANALOG_FILTER_SHIFT_FACTOR,
          class FilterType = ANALOG_FILTER_TYPE, class AnalogType = analog_t,
          uint8_t IncRes = MaximumFilteredAnalogIncRes<
              FilterShiftFactor, FilterType, AnalogType>::value>
class FilteredAnalog
    : public GenericFilteredAnalog<AnalogType (*)(AnalogType), Precision,
                                   FilterShiftFactor, FilterType, AnalogType,
                                   IncRes> {
  public:
    /**
     * @brief   Construct a new FilteredAnalog object.
     *
     * @param   analogPin
     *          The analog pin to read from.
     * @param   initial 
     *          The initial value of the filter.
     */
    FilteredAnalog(pin_t analogPin, AnalogType initial = 0)
        : GenericFilteredAnalog<AnalogType (*)(AnalogType), Precision,
                                FilterShiftFactor, FilterType, AnalogType,
                                IncRes>(analogPin, nullptr, initial) {}

    /**
     * @brief   Construct a new FilteredAnalog object.
     * 
     * **This constructor should not be used.**  
     * It is just a way to easily create arrays of FilteredAnalog objects, and
     * initializing them later. Trying to update a default-constructed or 
     * uninitialized FilteredAnalog object will result in a fatal runtime error.
     */
    FilteredAnalog() : FilteredAnalog(NO_PIN) {}

    /// A function pointer to a mapping function to map analog values.
    /// @see    map()
    using MappingFunction = AnalogType (*)(AnalogType);

    /**
     * @brief   Invert the analog value. For example, if the precision is 10 
     *          bits, when the analog input measures 1023, the output will be 0,
     *          and when the analog input measures 0, the output will be 1023.
     * 
     * @note    This overrides the mapping function set by the `map` method.
     */
    void invert() {
        constexpr AnalogType maxval = FilteredAnalog::getMaxRawValue();
        this->map([](AnalogType val) -> AnalogType { return maxval - val; });
    }
};

END_AH_NAMESPACE

AH_DIAGNOSTIC_POP()
