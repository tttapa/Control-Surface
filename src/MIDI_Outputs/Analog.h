#ifndef ANALOG_h_
#define ANALOG_h_

// #include <Arduino.h>
#include "../Settings/Settings.h"
#include "../Helpers/AnalogFilter.h"
#include "./MIDI_Control_Element.h"
#include "../ExtendedInputOutput/ExtendedInputOutput.h"

/**
 * A pure virtual class for MIDI_Control_Element%s that read from an analog input.
 * 
 * The analog input value is filtered using an exponential moving average filter. The settings for this filter can be changed in Settings.h.
 * A map function can be applied to the analog value (e.g. to compensate for logarithmic taper potentiometers or to calibrate the range).
 * No specific function for sending the value as a MIDI message is implemented. Classes that inherit from this class should implement the send function.
 */
class AnalogBase : public MIDI_Control_Element
{
public:
  AnalogBase(pin_t analogPin); // Constructor
  void map(int (*fn)(int));    // Change the function pointer for analogMap to a new function. It will be applied to the raw analog input value in Analog::refresh()

private:
  void refresh(); // Read the analog input value, update the average, map it to a MIDI value, check if it changed since last time, if so, send Control Change message over MIDI

  pin_t analogPin;
  uint8_t controllerNumber, channel;
  int (*mapFn)(int) = nullptr;
  AnalogFilter<ANALOG_FILTER_SHIFT_FACTOR> filter;

protected:
  virtual void send(unsigned int value) = 0;
};

#define Analog _Pragma ("GCC warning \"'Analog' is deprecated\"") AnalogCC

class AnalogCC : public AnalogBase
{
public:
  AnalogCC(pin_t analogPin, uint8_t controller, uint8_t channel)
      : AnalogBase(analogPin), controller(controller), channel(channel) {}

protected:
  virtual void send(unsigned int value);
  uint8_t oldVal = -1;

  uint8_t controller, channel;
};

#define AnalogHiRes _Pragma ("GCC warning \"'AnalogHiRes' is deprecated\"") AnalogPB

class AnalogPB : public AnalogBase
{
public:
  AnalogPB(pin_t analogPin, uint8_t channel)
      : AnalogBase(analogPin), channel(channel) {}

protected:
  virtual void send(unsigned int value);
  uint16_t oldVal = -1;

  uint8_t channel;
};

#endif // ANALOG_h_
