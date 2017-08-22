#include "Digital.h"
#include "MIDIOutput.h"

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
  bool value = digitalRead(pin) ^ invert; // read the button state and invert it if "invert" is true
  if (value != oldVal)                    // if the state changed since last time
  {
    if (value == LOW) // If the button is pressed
      sendMIDI(NOTE_ON, channel + channelOffset, note + addressOffset, velocity);
    else // If the button is not pressed
      sendMIDI(NOTE_OFF, channel + channelOffset, note + addressOffset, velocity);
    oldVal = value;
  }
}