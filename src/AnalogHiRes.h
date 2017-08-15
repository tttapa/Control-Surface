#ifndef AnalogHiRes_h_
#define AnalogHiRes_h_

#include "Arduino.h"
#include "USBMidi.h"

class AnalogHiRes
{
public:
  AnalogHiRes(uint8_t analogPin, uint8_t channel); // Constructor
  ~AnalogHiRes();                                  // Deconstructor
  void average(size_t length);                     // Use the average of multiple samples of analog readings
  void refresh();                                  // Read the analog input value, update the average, map it to a MIDI value, check if it changed since last time, if so, send Pitch Bend message over MIDI
  void map(uint16_t (*fn)(uint16_t));              // Change the function pointer for analogMap to a new function. It will be applied to the raw analog input value in Analog::refresh()
  void bank(uint8_t bankPin, uint8_t altChannel);  // Enable the bank mode. When bank switch is turned on, send on the alternative MIDI channel
  void detachBank();                               // Disable the bank mode

private:
  uint8_t analogPin, channel, bankPin, altChannel;
  uint16_t value;
  uint16_t oldVal = -1;
  bool bankEnabled = false;
  uint16_t (*analogMap)(uint16_t) = [](uint16_t x) { return x; }; // function pointer to identity function f(x) → x
  size_t avLen = 0;
  unsigned int *avValues;
  uint8_t avIndex = 0;
  unsigned long long avSum = 0;
  uint8_t avCount = 0;
  unsigned int runningAverage(unsigned int value); // http://playground.arduino.cc/Main/RunningAverage
};

#endif // AnalogHiRes_h_
