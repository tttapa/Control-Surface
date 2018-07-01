#include "./FilteredAnalog.h"
#include "../Control_Surface/Control_Surface_Class.h"

using namespace ExtIO;

FilteredAnalog::FilteredAnalog(pin_t analogPin) : analogPin(analogPin) {} // Constructor

bool FilteredAnalog::update()
{
  unsigned int input = ExtIO::analogRead(analogPin);   // read the raw analog input value
  if (mapFn != nullptr)                                // if a map function is specified
    input = mapFn(input);                              // apply the map function to the value
  input = filter.filter(input);                        // apply a low-pass EMA filter
  uint8_t newValue = hysteresis.getOutputLevel(input); // map from the 10-bit analog input value [0, 1023] to the 7-bit MIDI value [0, 127]
  bool changed = newValue != value;
  value = newValue;
  return changed;
}

uint8_t FilteredAnalog::getValue()
{
  return value;
}

void FilteredAnalog::map(int (*fn)(int)) // change the function pointer for analogMap to a new function. It will be applied to the raw analog input value in FilteredAnalog::update()
{
  mapFn = fn;
}