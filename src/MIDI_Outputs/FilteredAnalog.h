#ifndef FILTEREDANALOG_H_
#define FILTEREDANALOG_H_

#include "../Settings/SettingsWrapper.h"
#include "../Helpers/EMA.h"
#include "../Helpers/Hysteresis.h"
#include "../ExtendedInputOutput/ExtendedInputOutput.h"

/**
 * A class that reads and filters an analog input.
 * 
 * A map function can be applied to the analog value (e.g. to compensate for logarithmic taper potentiometers or to calibrate the range).  
 * The analog input value is filtered using an exponential moving average filter. The settings for this filter can be changed in Settings.h.  
 */
class FilteredAnalog
{
public:
  /**
   * @brief Construct a new FilteredAnalog object.
   * 
   * @param analogPin 
   *        The analog pin to read from.
   */
  FilteredAnalog(pin_t analogPin); // Constructor
  /**
   * @brief Specify a mapping function that is applied to the raw analog value before sending.
   * 
   * @param fn 
   *        A function pointer to the mapping function. This function should take the 10-bit analog value as a parameter, and should return a 10-bit value.
   */
  void map(int (*fn)(int));

  /**
   * @brief Read the analog input value, apply the mapping function, and update the average.
   * 
   * @return bool
   *         True if the value changed since last time it was updated, false otherwise.
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
  Hysteresis hysteresis;

  uint8_t value = 0xFF;
};

#endif // FILTEREDANALOG_H_
