#ifndef AnalogHiRes_h_
#define AnalogHiRes_h_

#include "Arduino.h"
#include "./MIDI_Control_Element.h"
#include "../ExtendedInputOutput/ExtendedInputOutput.h"
#include "../Settings/Settings.h"

class AnalogHiRes : public MIDI_Control_Element
{
public:
  AnalogHiRes(pin_t analogPin, uint8_t channel); // Constructor
  void map(int (*fn)(int));                        // Change the function pointer for analogMap to a new function. It will be applied to the raw analog input value in Analog::refresh()

private:
  void refresh(); // Read the analog input value, update the average, map it to a MIDI value, check if it changed since last time, if so, send Pitch Bend message over MIDI

  pin_t analogPin;
  uint8_t channel;
  uint16_t oldVal = -1;
  bool bankEnabled = false;
  int (*analogMap)(int) = identity; // function pointer to identity function f(x) → x
  unsigned int avValues[ANALOG_HI_RES_AVERAGE];
  uint8_t avIndex = 0;
  unsigned long long avSum = 0;
  uint8_t avCount = 0;

  static int identity(int x) // identity function f(x) → x
  {
    return x;
  }

  unsigned int runningAverage(unsigned int value); // http://playground.arduino.cc/Main/RunningAverage
};

#endif // AnalogHiRes_h_
