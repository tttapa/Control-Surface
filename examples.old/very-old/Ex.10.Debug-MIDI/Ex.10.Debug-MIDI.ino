/**
 * @example Ex.10.Debug-MIDI.ino
 * 
 * This is an example of the debug functionality of the Control_Surface library.
 * 
 * Connect a push buttons to digital pin 2. Connect the other pin of the button to the ground, 
 * a pull-up resistor is not necessary, because the internal one will be used.  
 * This button will play MIDI note C4 when pressed.  
 * 
 * Open the Serial Monitor in the Arduino IDE (CTRL+SHIFT+M) and press the button.  
 * You should see messages like:
 *
 * - Note On    Channel: 1  Data 1: 0x3C  Data 2: 0x7F
 * - Note Off    Channel: 1  Data 1: 0x3C  Data 2: 0x7F
 * 
 * You can also send MIDI messages in hexadecimal format:  
 * For example: `90 3C 7F` (note on, note 0x3C, velocity 0x7F) should turn on the built-in LED.  
 * `80 3C 7F` (note off, note 0x3C, velocity 0x7F) should turn off the built-in LED.
 *
 * Written by Pieter P, 20-09-2017  
 * https://github.com/tttapa/Control-Surface
 */

#include <Control_Surface.h> // Include the library

// Instantiate a MIDI interface that sends debug messages to the USB Serial port (Serial Monitor)
USBDebugMIDI_Interface midiInterface(115200);

const uint8_t velocity = 0b1111111; // Maximum velocity (0b1111111 = 0x7F = 127)
const uint8_t C4 = 60;              // Note number 60 is defined as middle C in the MIDI specification

// Create a new instance of the class 'Digital', called 'button', on pin 2, that sends MIDI messages with note 'C4' (60) on channel 1, with velocity 127
Digital button(2, C4, 1, velocity);

// Create a new instance of the class 'MIDI_LED', called 'led' on the built-in LED, that reacts to MIDI messages with note 'C4' (60) on channel 1.
MIDI_LED led(LED_BUILTIN, C4, 1);


void setup() {}

void loop() {
  // Refresh the MIDI controller (check whether the potentiometer's input has changed since last time, if so, send the new value over MIDI)
  Control_Surface.refresh();
}

/**
   If you don't explicitly instantiate a MIDI interface, the MIDI Controller library will
   instantiate a MIDI interface that sends MIDI messages over the USB connection.
   
   If you do explicitly instantiate a MIDI interface, like in this example,
   it will be used as the default MIDI interface by the MIDI Controller library.
   
   If you instantiate more than one MIDI interfaces, the interface that's the last
   one to be instantiated will be the default output, unless you specify a default
   interface using MIDI_Interface.setDefault().
*/
