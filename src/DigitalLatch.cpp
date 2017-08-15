#include "DigitalLatch.h"

DigitalLatch::DigitalLatch(uint8_t pin, uint8_t note, uint8_t channel, uint8_t velocity, unsigned long latchTime) // Constructor
{
  pinMode(pin, INPUT_PULLUP); // Enable the internal pull-up resistor on the pin with the button/switch
  this->pin = pin;
  this->note = note;
  this->channel = channel;
  this->velocity = velocity;
  this->latchTime = latchTime;
}

DigitalLatch::~DigitalLatch() // Deconstructor
{
  pinMode(pin, INPUT);       // make it a normal input again, without the internal pullup resistor.
  if (bankEnabled)           // if bank mode was used
    pinMode(bankPin, INPUT); // make make the bank switch pin a normal input again, without the internal pullup resistor.
}

void DigitalLatch::refresh() // Check if the button state changed, if so, send a MIDI Note On, after a non-blocking delay of "latchTime", send a Note Off
{
  bool state = digitalRead(pin); // read the button state
  if (state != oldState)         // If the switch changed position
  {
    if (noteOffSent) // If the note is turned off
    {
      sendNote(NOTE_ON);     // Turn on the note
      noteOnTime = millis(); // store the time of the note on message
      noteOffSent = false;   // The note is turned on
    }
    else // If the button is switched again, before latch time is reached
    {
      sendNote(NOTE_OFF);    // Turn off the note
      sendNote(NOTE_ON);     // Immediately turn the note on again
      noteOnTime = millis(); // store the time of the note on message
    }
    oldState = state;
  }
  if (millis() - noteOnTime > latchTime && !noteOffSent) // if the time elapsed since the Note On event is greater than the latch time, and if the note is still on
  {
    sendNote(NOTE_OFF); // Turn off the note
    noteOffSent = true; // The note is turned off
  }
}

void DigitalLatch::bank(uint8_t bankPin, uint8_t altNote, uint8_t altChannel) // Enable the bank mode. When bank switch is turned on, send alternative MIDI channel and controller numbers
{                                                                             // bankPin = digital pin with toggle switch connected
  bankEnabled = true;

  this->bankPin = bankPin;
  pinMode(bankPin, INPUT_PULLUP);
  this->altNote = altNote;
  this->altChannel = altChannel;
}

void DigitalLatch::detachBank() // Disable the bank mode
{
  if (bankEnabled) // only defined if bank mode is enabled
  {
    bankEnabled = false;
    pinMode(bankPin, INPUT); // make it a normal input again, without the internal pullup resistor
  }
}

void DigitalLatch::sendNote(uint8_t noteOnOrOff) // turn on or off a note, select the channel and note number based on the bank mode and bank switch state 
{
  if (bankEnabled && !digitalRead(bankPin))                             // if the bank mode is enabled, and the bank switch is in the 'alternative' position (i.e. if the switch is on (LOW))
    USBMidiController.send(noteOnOrOff, altChannel, altNote, velocity); // send a Note On or Off MIDI event with the 'alternative' channel and note number
  else                                                                  // if the bank mode is disabled, or the bank switch is in the normal position
    USBMidiController.send(noteOnOrOff, channel, note, velocity);       // send a Note On or Off MIDI event with the normal, original channel and note number
}