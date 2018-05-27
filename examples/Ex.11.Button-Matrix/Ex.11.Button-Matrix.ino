/**
 * @example Ex.11.Button-Matrix.ino
 * 
 * This is an example of the ButtonMatrix class of the Control_Surface library.
 * 
 * Connect a 4 × 3 matrix of buttons with the rows to pins 2, 3, 4 and 5, 
 * and the columns to pins 6, 7 and 8.  
 * Pull-up resistors are not necessary, because the internal ones will be used. 
 * 
 * If you want to be able to press multiple buttons at once, add a diode 
 * in series with each button, as shown in the following schematic:
 * 
 * @image html Button-matrix.svg
 * 
 * The note numbers are specified in the `addresses` array.  
 * Map accordingly in your DAW or DJ software.
 * 
 * Written by tttapa, 24/09/2017  
 * https://github.com/tttapa/Control-Surface
*/

#include "Control_Surface.h" // Include the library

const uint8_t velocity = 0b1111111; // Maximum velocity (0b1111111 = 0x7F = 127)
const uint8_t addresses[4][3] = {   // the note numbers corresponding to the buttons in the matrix
  {  1,  2,  3 },
  {  4,  5,  6 },
  {  7,  8,  9 },
  { 10, 11, 12 }
};

// Create a new instance of the class 'ButtonMatrix', called 'buttonmatrix', with dimensions 4 rows and 3 columns, with the rows connected to pins 2, 3, 4 and 5
// and the columns connected to pins 6, 7 and 8, that sends MIDI messages with the notes specified in 'addresses' on MIDI channel 1, with velocity 127
ButtonMatrix<4, 3> buttonmatrix( {2, 3, 4, 5}, {6, 7, 8}, addresses, 1, velocity);

void setup() {}

void loop() {
  // Refresh the buttons (check whether the states have changed since last time, if so, send it over MIDI)
  Control_Surface.refresh();
}
