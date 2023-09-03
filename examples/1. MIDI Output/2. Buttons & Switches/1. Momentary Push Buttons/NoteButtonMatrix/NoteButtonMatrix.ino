/**
 * This examples shows how to use a button matrix to read many switches.
 * 
 * @boards  AVR, AVR USB, Nano Every, Due, Nano 33 IoT, Nano 33 BLE, UNO R4, Pi Pico, Teensy 3.x, ESP32, ESP8266
 * 
 * Connections
 * -----------
 * 
 * Connect a 4 × 3 matrix of buttons with the rows to pins 2, 3, 4 and 5, 
 * and the columns to pins 6, 7 and 8.
 * 
 * Pull-up resistors are not necessary, because the internal ones will be used. 
 * 
 * If you want to be able to press multiple buttons at once, add a diode 
 * in series with each button:  
 * @image html Button-matrix.png
 * 
 * Behavior
 * --------
 * 
 * When you press one of the buttons, a note on event for the corresponding note
 * is sent, when you release it, a note off event is sent.
 * 
 * Mapping
 * -------
 *
 * The note numbers are specified in the `addresses` array.
 * Map accordingly in your DAW or DJ software.
 *
 * Written by Pieter P, 24/09/2017  
 * https://github.com/tttapa/Control-Surface
 */

#include <Control_Surface.h>

USBMIDI_Interface midi;

// The note numbers corresponding to the buttons in the matrix
const AddressMatrix<4, 3> addresses {{
  {1, 2, 3},
  {4, 5, 6},
  {7, 8, 9},
  {10, 11, 12},
}};

NoteButtonMatrix<4, 3> buttonmatrix {
  {2, 3, 4, 5}, // row pins
  {6, 7, 8},    // column pins
  addresses,    // address matrix
  Channel_1,    // channel and cable number
};

void setup() {
  Control_Surface.begin();
}

void loop() {
  Control_Surface.loop();
}