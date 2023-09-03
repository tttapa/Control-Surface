/** 
 * This is an example that demonstrates the use of MIDI Program Changers.
 *
 * @boards  AVR, AVR USB, Nano Every, Due, Nano 33 IoT, Nano 33 BLE, UNO R4, Pi Pico, Teensy 3.x, ESP32, ESP8266
 * 
 * Connections
 * -----------
 * 
 * - 2: momentary push button (to ground)
 * - 3: momentary push button (to ground)
 * - 4: momentary push button (to ground)
 * 
 * The internal pull-up resistor for the buttons will be enabled automatically.
 * 
 * Behavior
 * --------
 * 
 * - When the button on pin 2 is pressed, the MIDI Program for 
 *   "Acoustic Grand Piano" is selected.
 * - When the button on pin 3 is pressed, the MIDI Program for 
 *   "Rock Organ" is selected.
 * - When the button on pin 4 is pressed, the MIDI Program for 
 *   "Electric Bass Pick" is selected. 
 * 
 * Written by PieterP, 2019-08-07  
 * https://github.com/tttapa/Control-Surface
 */

#include <Control_Surface.h> // Include the Control Surface library

// Instantiate a MIDI over USB interface
USBMIDI_Interface midi;

// Instantiate a program changer with 3 programs
ProgramChanger<3> programChanger {
  {
    MIDI_PC::Acoustic_Grand_Piano, // list of programs
    MIDI_PC::Rock_Organ,
    MIDI_PC::Electric_Bass_Pick,
  },
  Channel_1, // MIDI channel to use
};

// Instantiate a selector that reads three buttons and controls the program
// changer
ManyButtonsSelector<3> programSelector {
  programChanger,
  {{2, 3, 4}},
};

// The library comes with many different Selectors, like rotary encoders,
// push buttons that increment or decrement the setting, versions with LEDs
// to display the current setting, etc.
// https://tttapa.github.io/Control-Surface-doc/Doxygen/d0/d0c/group__Selectors.html

void setup() {
  Control_Surface.begin(); // Initialize Control Surface
}

void loop() {
  Control_Surface.loop(); // Update the Control Surface
}
