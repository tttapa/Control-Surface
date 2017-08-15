#include "Digital.h"

Digital::Digital(uint8_t pin, uint8_t note, uint8_t channel, uint8_t velocity) // Constructor
{
  pinMode(pin, INPUT_PULLUP); // Enable the internal pull-up resistor on the pin with the button/switch
  this->pin = pin;
  this->note = note;
  this->channel = channel;
  this->velocity = velocity;
}

Digital::~Digital() // Deconstructor
{
  pinMode(pin, INPUT);       // make it a normal input again, without the internal pullup resistor.
  if (bankEnabled)           // if bank mode was used
    pinMode(bankPin, INPUT); // make make the bank switch pin a normal input again, without the internal pullup resistor.
}

void Digital::refresh() // Check if the button state changed, and send a MIDI Note On or Off accordingly
{
  bool value = digitalRead(pin) ^ invert; // read the button state and invert it if "invert" is true
  if (value != oldVal)                    // if the state changed since last time
  {
    if (value == LOW) // If the button is pressed
    {
      if (bankEnabled && !digitalRead(bankPin))                         // if the bank mode is enabled, and the bank switch is in the 'alternative' position (i.e. if the switch is on (LOW))
        USBMidiController.send(NOTE_ON, altChannel, altNote, velocity); // send a Note On MIDI event with the 'alternative' channel and note number
      else                                                              // if the bank mode is disabled, or the bank switch is in the normal position
        USBMidiController.send(NOTE_ON, channel, note, velocity);       // send a Note On MIDI event with the normal, original channel and note number
    }
  }
  else // If the button is not pressed
  {
    if (bankEnabled && !digitalRead(bankPin))                          // if the bank mode is enabled, and the bank switch is in the 'alternative' position (i.e. if the switch is on (LOW))
      USBMidiController.send(NOTE_OFF, altChannel, altNote, velocity); // send a Note Off MIDI event with the 'alternative' channel and note number
    else                                                               // if the bank mode is disabled, or the bank switch is in the normal position
      USBMidiController.send(NOTE_OFF, channel, note, velocity);       // send a Note Off MIDI event with the normal, original channel and note number
  }
  oldVal = value;
}

void Digital::bank(uint8_t bankPin, uint8_t altNote, uint8_t altChannel) // Enable the bank mode. When bank switch is turned on, send alternative MIDI channel and controller numbers
{                                                                        // bankPin = digital pin with toggle switch connected
  bankEnabled = true;

  this->bankPin = bankPin;
  pinMode(bankPin, INPUT_PULLUP);
  this->altNote = altNote;
  this->altChannel = altChannel;
}

void Digital::detachBank() // Disable the bank mode
{
  if (bankEnabled) // only defined if bank mode is enabled
  {
    bankEnabled = false;
    pinMode(bankPin, INPUT); // make it a normal input again, without the internal pullup resistor
  }
}
