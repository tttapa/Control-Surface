#ifndef ANALOG_h_
#define ANALOG_h_

#include "Arduino.h"
#include "MIDI_Control_Element.h"

class Analog : public MIDI_Control_Element
{
public:
  Analog(uint8_t analogPin, uint8_t controllerNumber, uint8_t channel); // Constructor
  ~Analog();                                                            // Destructor
  void average(size_t length);                                          // Use the average of multiple samples of analog readings
  void refresh();                                                       // Read the analog input value, update the average, map it to a MIDI value, check if it changed since last time, if so, send Control Change message over MIDI
  void map(int (*fn)(int));                                             // Change the function pointer for analogMap to a new function. It will be applied to the raw analog input value in Analog::refresh()

private:
  uint8_t analogPin, controllerNumber, channel, oldVal = -1;
  bool bankEnabled = false;
  int (*analogMap)(int) = identity; // function pointer to identity function f(x) → x
  size_t avLen = 0;
  unsigned int *avValues = nullptr;
  size_t avIndex = 0;
  unsigned long avSum = 0;
  size_t avCount = 0;

  static int identity(int x) { // identity function f(x) → x
    return x;
  }

  unsigned int runningAverage(unsigned int value); // http://playground.arduino.cc/Main/RunningAverage
};

#endif // ANALOG_h_
