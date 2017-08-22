/*
  This is an example of the "Digital" class of the MIDI_controller library.
  Connect a push buttons to digital pin 2. Connect the other pin of the button to the ground,
  a pull-up resistor is not necessary, because the internal one will be used.
  This button will play MIDI note C4 when pressed.
  Map it in your DAW or DJ software.

  Written by tttapa, 21/08/2015
  https://github.com/tttapa/MIDI_controller
*/

#include <MIDI_controller.h> // Include the library

USBMIDIOutput midiout; // Instantiate a USB MIDI output

const uint8_t dataPin = 12;
const uint8_t clockPin = 10;
const uint8_t latchPin = 11;

ShiftRegisterOut SR(dataPin, clockPin, latchPin, LSBFIRST, 24);

const uint8_t REC_RDY = 0;
const uint8_t SOLO = 8;
const uint8_t MUTE = 16;
const uint8_t SELECT = 24;

const uint8_t PAN = 42;
const uint8_t VPOT = 16;



Digital buttons[] = {
  {2, MUTE, 1, 127},
  {3, SOLO, 1, 127},
  {4, REC_RDY, 1, 127},
  {7, SELECT, 1, 127},

  {11, PAN, 1, 127},
};

Bank b;
BankSelector bs(b, { 8, 9 }, {
  SR.red(0),
  SR.red(1),
  SR.red(2),
  SR.red(3),
  SR.red(4),
  SR.red(5),
  SR.red(6),
  SR.red(7),
});                             // INCREMENT_DECREMENT_LEDS

RotaryEncoder encoder(0, 1, VPOT, 1, 1, NORMAL_ENCODER, MACKIE_CONTROL_RELATIVE);


const size_t nb_buttons = sizeof(buttons) / sizeof(Digital);

void setup()
{
  startMIDI(); // Initialise the MIDI output connection
  for (uint8_t i = 0; i < nb_buttons; i++)
    b.add(buttons[i], Bank::CHANGE_ADDRESS);
}

void loop()
{
  for (uint8_t i = 0; i < nb_buttons; i++)
    buttons[i].refresh();
  encoder.refresh();
}
