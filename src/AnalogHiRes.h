#ifndef AnalogHiRes_h_
#define AnalogHiRes_h_

#include "Arduino.h"
#include "USBMidi.h"
#include "MIDI_Control_Element.h"

// #define PITCH_BEND_RESOLUTION_14_BITS // use true 14-bit resolution instead of 10-bit resolution padded with four zeros. (only on boards that support it, e.g. Teensy and Arduino Due)

class AnalogHiRes : public MIDI_Control_Element
{
public:
  AnalogHiRes(uint8_t analogPin, uint8_t channel); // Constructor
  ~AnalogHiRes();                                  // Destructor
  void average(size_t length);                     // Use the average of multiple samples of analog readings
  void refresh();                                  // Read the analog input value, update the average, map it to a MIDI value, check if it changed since last time, if so, send Pitch Bend message over MIDI
  void map(int (*fn)(int));                        // Change the function pointer for analogMap to a new function. It will be applied to the raw analog input value in Analog::refresh()

private:
  uint8_t analogPin, channel;
  uint16_t oldVal = -1;
  bool bankEnabled = false;
  int (*analogMap)(int) = identity; // function pointer to identity function f(x) → x
  size_t avLen = 0;
  unsigned int *avValues;
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
