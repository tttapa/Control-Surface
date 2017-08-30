#include "ControlChange.h"
#include "MIDI_controller.h"

ControlChange::ControlChange(uint8_t controllerNumber, uint8_t channel) // Constructor
{
  this->controllerNumber = controllerNumber;
  this->channel = channel;
}

ControlChange::~ControlChange() // Destructor
{
  free(avValues);            // free the sample buffer malloc'ed in Analog::average
  if (bankEnabled)           // if bank mode was used
    pinMode(bankPin, INPUT); // make make the bank switch pin a normal input again, without the internal pullup resistor.
}

void ControlChange::average(size_t length) // use the average of multiple samples of analog readings
{
  if (length == 0 || length == 1 || avLen) // the average of 0 or 1 samples is meaningless, if "av" already exists, don't allocate new memory
    return;
  avValues = (unsigned int *)malloc(length * sizeof(unsigned int)); // allocate memory for the sample buffer
  memset(avValues, 0, length * sizeof(unsigned int));               // set all values in the buffer to zero
  avLen = length;
}

void ControlChange::refresh(uint8_t value) // Update the controller with a new value between 0 and 127, update the average, check if the MIDI value changed since last time, if so, send Control Change message over MIDI
{
  if (avLen)                       // if averaging is enabled
    value = runningAverage(value); // update the running average with the new value
  if (value != oldVal)             // if the value changed since last time
  {
    if (bankEnabled && !digitalRead(bankPin))                       // if the bank mode is enabled, and the bank switch is in the 'alternative' position (i.e. if the switch is on (LOW))
      MIDI_Controller.MIDI()->send(CC, altChannel, altController, value); // send a Control Change MIDI event with the 'alternative' channel and controller number
    else                                                            // if the bank mode is disabled, or the bank switch is in the normal position
      MIDI_Controller.MIDI()->send(CC, channel, controllerNumber, value); // send a Control Change MIDI event with the normal, original channel and controller number
    oldVal = value;
  }
}

void ControlChange::refresh(float input) // Overload of the refresh() method that takes a float between 0.0 and 1.0 instead of an int between 0 and 127
{
  refresh((uint8_t)round(127.0 * input));
}

void ControlChange::bank(uint8_t bankPin, uint8_t altController, uint8_t altChannel) // Enable the bank mode. When bank switch is turned on, send alternative MIDI channel and controller numbers
{                                                                                    // bankPin = digital pin with toggle switch connected
  bankEnabled = true;

  this->bankPin = bankPin;
  pinMode(bankPin, INPUT_PULLUP);
  this->altController = altController;
  this->altChannel = altChannel;
}

void ControlChange::detachBank() // Disable the bank mode
{
  if (bankEnabled) // only defined if bank mode is enabled
  {
    bankEnabled = false;
    pinMode(bankPin, INPUT); // make it a normal input again, without the internal pullup resistor.
  }
}

unsigned int ControlChange::runningAverage(unsigned int value) // http://playground.arduino.cc/Main/RunningAverage
{
  avSum -= avValues[avIndex];
  avValues[avIndex] = value;
  avSum += value;
  avIndex++;
  avIndex = avIndex % avLen;
  if (avCount < avLen)
    avCount++;
  return avSum / avCount;
}