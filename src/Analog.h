#ifndef ANALOG_h_
#define ANALOG_h_

#include "Arduino.h"
#include "USBMidi.h"

class Analog
{
public:
  Analog(uint8_t analogPin, uint8_t controllerNumber, uint8_t channel); // Constructor
  ~Analog(); // Deconstructor
  void average(size_t length); // Use the average of multiple samples of analog readings
  void refresh(); // Read the analog input value, update the average, map it to a MIDI value, check if it changed since last time, if so, send Control Change message over MIDI
  void map(int (*fn)(int)); // change the function pointer for analogMap to a new function. It will be applied to the raw analog input value in Analog::refresh()
  void bank(uint8_t bankPin, uint8_t altController, uint8_t altChannel); // Enable the bank mode. When bank switch is turned on, send alternative MIDI channel and controller numbers
  void detachBank(); // Disable the bank mode

private:
  uint8_t analogPin, controllerNumber, channel, bankPin, altController, altChannel, value, oldVal = -1;
  bool bankEnabled = false;
  int (*analogMap)(int) = [](int x) { return x; }; // function pointer to identity function f(x) → x
  size_t avLen = 0;
  unsigned int *avValues = nullptr;
  size_t avIndex = 0;
  unsigned long avSum = 0;
  size_t avCount = 0;

  unsigned int runningAverage(unsigned int value); // http://playground.arduino.cc/Main/RunningAverage
};

#endif // ANALOG_h_
