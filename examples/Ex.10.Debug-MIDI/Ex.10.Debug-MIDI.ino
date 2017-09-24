/*
  This is an example of the debug functionality of the MIDI_controller library.
  Connect a potentiometer to analog pin A0. This will be the MIDI channel volume of channel 1.

  Open the Serial Monitor in the Arduino IDE (CTRL+SHIFT+M) and move the potentiometer.
  You should see messages like:
  Control Change    Channel: 1  Data 1: 0x7  Data 2: 0x1
  Control Change    Channel: 1  Data 1: 0x7  Data 2: 0x2
  Control Change    Channel: 1  Data 1: 0x7  Data 2: 0x3

  Written by Pieter P, 20-09-2017
  https://github.com/tttapa/MIDI_controller
*/

#include <MIDI_Controller.h> // Include the library

// Instantiate a MIDI interface that sends debug messages to the USB Serial port (Serial Monitor)
USBDebugMIDI_Interface midiInterface(115200);

// Create a new instance of the class 'Analog', called 'potentiometer', on pin A0,
// that sends MIDI messages with controller 7 (channel volume) on channel 1
Analog potentiometer(A0, MIDI_CC::Channel_Volume, 1);

void setup() {}

void loop() {
  // Refresh the MIDI controller (check whether the potentiometer's input has changed since last time, if so, send the new value over MIDI)
  MIDI_Controller.refresh();
}

/*
   If you don't explicitly instantiate a MIDI interface, the MIDI Controller library will
   instantiate a MIDI interface that sends MIDI messages over the USB connection.
   
   If you do explicitly instantiate a MIDI interface, like in this example,
   it will be used as MIDI output by the MIDI Controller library.
   
   If you instantiate more than one MIDI interfaces, the interface that's the last
   one to be instantiated will be the default output, unless you specify a default
   interface using MIDI_Interface.setDefault().
*/
