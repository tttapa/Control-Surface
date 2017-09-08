#ifndef ANALOG_h_
#define ANALOG_h_

#include "Arduino.h"
#include "../Settings/Settings.h"
#include "MIDI_Control_Element.h"

class Analog : public MIDI_Control_Element
{
public:
  Analog(uint8_t analogPin, uint8_t controllerNumber, uint8_t channel); // Constructor
  ~Analog();                                                            // Destructor
#ifdef SINGLE_BYTE_AVERAGE
  void average(uint8_t length); // Use the average of multiple samples of analog readings
#else
  void average(size_t length);                     // Use the average of multiple samples of analog readings
#endif
  void map(int (*fn)(int)); // Change the function pointer for analogMap to a new function. It will be applied to the raw analog input value in Analog::refresh()

private:
  void refresh();           // Read the analog input value, update the average, map it to a MIDI value, check if it changed since last time, if so, send Control Change message over MIDI
  
  uint8_t analogPin, controllerNumber, channel, oldVal = -1;
  bool bankEnabled = false;
  int (*analogMap)(int) = identity; // function pointer to identity function f(x) → x

  static int identity(int x)
  { // identity function f(x) → x
    return x;
  }

#ifdef SINGLE_BYTE_AVERAGE
  uint8_t runningAverage(uint8_t value); // http://playground.arduino.cc/Main/RunningAverage
  uint8_t avLen = 0;
  uint8_t *avValues = nullptr;
  uint8_t avIndex = 0;
  unsigned int avSum = 0;
  uint8_t avCount = 0;
#else
  unsigned int runningAverage(unsigned int value); // http://playground.arduino.cc/Main/RunningAverage
  size_t avLen = 0;
  unsigned int *avValues = nullptr;
  size_t avIndex = 0;
  unsigned long avSum = 0;
  size_t avCount = 0;
#endif
};

#endif // ANALOG_h_
