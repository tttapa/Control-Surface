#ifndef FILTEREDANALOG_H_
#define FILTEREDANALOG_H_

#include "../Hardware/ExtendedInputOutput/ExtendedInputOutput.h"
#include "../Helpers/EMA.h"
#include "../Helpers/Hysteresis.h"
#include "../Settings/SettingsWrapper.h"

/**
 * A class that reads and filters an analog input.
 *
 * A map function can be applied to the analog value (e.g. to compensate for
 * logarithmic taper potentiometers or to calibrate the range). The analog input
 * value is filtered using an exponential moving average filter. The settings
 * for this filter can be changed in Settings.h.
 */
template <uint8_t HYSTERESIS_BITS> class FilteredAnalog {
public:
  /**
   * @brief Construct a new FilteredAnalog object.
   *
   * @param analogPin
   *        The analog pin to read from.
   */
  FilteredAnalog(pin_t analogPin); // Constructor
  /**
   * @brief Specify a mapping function that is applied to the raw analog value
   * before sending.
   *
   * @param fn
   *        A function pointer to the mapping function. This function should
   * take the 10-bit analog value as a parameter, and should return a 10-bit
   * value.
   */
  void map(int (*fn)(int));

  /**
   * @brief Read the analog input value, apply the mapping function, and update
   * the average.
   *
   * @return bool
   *         True if the value changed since last time it was updated, false
   * otherwise.
   */
  bool update();

  /**
   * @brief Get the filtered value of the analog input.
   *
   * @return uint8_t
   *         The filtered value of the analog input, as a 7-bit number. [0, 127]
   *         0xFF if the update() has not been called yet.
   */
  uint8_t getValue();

private:
  const pin_t analogPin;
  int (*mapFn)(int) = nullptr;
  EMA<ANALOG_FILTER_SHIFT_FACTOR, ANALOG_FILTER_TYPE> filter;
  Hysteresis<HYSTERESIS_BITS> hysteresis;

  uint8_t value = 0xFF;
};

// ------------------------ Implementations ------------------------ //

#include "../Control_Surface/Control_Surface_Class.h"

using namespace ExtIO;

template <uint8_t HYSTERESIS_BITS>
FilteredAnalog<HYSTERESIS_BITS>::FilteredAnalog(pin_t analogPin)
    : analogPin(analogPin) {} // Constructor

template <uint8_t HYSTERESIS_BITS>
bool FilteredAnalog<HYSTERESIS_BITS>::update() {
  unsigned int input =
      ExtIO::analogRead(analogPin); // read the raw analog input value
  if (mapFn != nullptr)             // if a map function is specified
    input = mapFn(input);           // apply the map function to the value
  input = filter.filter(input);     // apply a low-pass EMA filter
  uint8_t newValue = hysteresis.getOutputLevel(
      input); // map from the 10-bit analog input value [0, 1023] to the 7-bit
              // MIDI value [0, 127]
  bool changed = newValue != value;
  value = newValue;
  return changed;
}

template <uint8_t HYSTERESIS_BITS>
uint8_t FilteredAnalog<HYSTERESIS_BITS>::getValue() {
  return value;
}

template <uint8_t HYSTERESIS_BITS>
void FilteredAnalog<HYSTERESIS_BITS>::map(int (*fn)(int)) {
  mapFn = fn;
}

#endif // FILTEREDANALOG_H_
