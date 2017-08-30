#include "Digital.h"
#include "MIDI_Interface.h"

Digital::Digital(uint8_t pin, uint8_t note, uint8_t channel, uint8_t velocity) // Constructor
{
  pinMode(pin, INPUT_PULLUP); // Enable the internal pull-up resistor on the pin with the button/switch
  this->pin = pin;
  this->note = note;
  this->channel = channel;
  this->velocity = velocity;
}

Digital::~Digital() // Destructor
{
  pinMode(pin, INPUT); // make it a normal input again, without the internal pullup resistor.
}

void Digital::refresh() // Check if the button state changed, and send a MIDI Note On or Off accordingly
{
  bool state = digitalRead(pin) ^ invert; // read the button state and invert it if "invert" is true

  if (millis() - prevBounceTime > debounceTime)
  {
    int8_t stateChange = state - buttonState;

    if (stateChange == falling)
    { // Button is pushed
      buttonState = state;
      MIDI_Controller.MIDI->send(NOTE_ON, channel + channelOffset, note + addressOffset, velocity);
    }

    if (stateChange == rising)
    { // Button is released
      buttonState = state;
      MIDI_Controller.MIDI->send(NOTE_OFF, channel + channelOffset, note + addressOffset, velocity);
    }
  }
  if (state != prevState)
  {
    prevBounceTime = millis();
    prevState = state;
  }
}