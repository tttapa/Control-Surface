#include "./Analog.h"
#include "../Control_Surface/Control_Surface_Class.h"

// AnalogBase
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- //

AnalogBase::AnalogBase(pin_t analogPin) : analogPin(analogPin) {} // Constructor

void AnalogBase::refresh() // read the analog value, update the average, map it to a MIDI value, check if it changed since last time, if so, send Control Change message over MIDI
{
  unsigned int input = analogRead(analogPin); // read the raw analog input value
  if (mapFn != nullptr)                       // if a map function is specified
    input = mapFn(input);                     // apply the map function to the value
  input = filter.filter(input);               // apply a low-pass EMA filter

  send(input);
}

void AnalogBase::map(int (*fn)(int)) // change the function pointer for analogMap to a new function. It will be applied to the raw analog input value in Analog::refresh()
{
  mapFn = fn;
}

// AnalogCC
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- //

void AnalogCC::send(unsigned int input)
{
  uint8_t value = input >> 3; // map from the 10-bit analog input value [0, 1023] to the 7-bit MIDI value [0, 127]
  if (value != oldVal)        // if the value changed since last time
  {
    Control_Surface.MIDI()->send(CC,
                                 channel + channelOffset * tracksPerBank,
                                 controller + addressOffset * tracksPerBank,
                                 value); // send a Control Change MIDI event
    oldVal = value;
  }
}

// AnalogPB
// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- //

void AnalogPB::send(unsigned int input)
{
  if (input != oldVal)
  {
    oldVal = input;
    input <<= 4; // Convert from 10 bit to 14-bit Pitch Bend data
    Control_Surface.MIDI()->send(PITCH_BEND,
                                 channel + channelOffset * tracksPerBank,
                                 input, input >> 7); // send a Pitch Bend MIDI event
  }
}
