/**
 * This example demonstrates the use of LEDs that respond to incoming  MIDI
 * note events by changing the brightness of an LED using PWM.
 * 
 * @boards  AVR, AVR USB, Nano Every, Due, Nano 33 IoT, Nano 33 BLE, UNO R4, Pi Pico, Teensy 3.x
 * 
 * Connections
 * -----------
 * 
 * Connect an LED (+ current limiting resistor) to any PWM pin. Then change the
 * pin number in the `ledPin` constant to match the pin with your LED connected.
 * 
 * Behavior
 * --------
 * 
 * If a MIDI Note On event for note 0x3C (C4 or middle C) is sent, the LED will
 * light up, with the brightness depending on the velocity.  
 * If a Note Off event for that note is sent, the LED will turn off.  
 * (A Note On event with a velocity of zero also counts as a Note Off event.)
 * 
 * Mapping
 * -------
 * 
 * Route the MIDI output of a MIDI keyboard to the Arduino's MIDI input. Then
 * play a middle C on the keyboard.
 * 
 * Written by PieterP, 2020-01-03  
 * https://github.com/tttapa/Control-Surface
 */

#include <Control_Surface.h> // Include the Control Surface library

// Instantiate a MIDI over USB interface.
USBMIDI_Interface midi;

const pin_t ledPin = LED_BUILTIN; // Change this to your PWM pin <------

// Instantiate the LED that will light up when middle C is playing
NoteLEDPWM led {
  ledPin,                        // Pin of the LED, must be PWM pin
  {MIDI_Notes::C[4], Channel_1}, // Note C4 on MIDI channel 1
};

void setup() {
  Control_Surface.begin(); // Initialize Control Surface
}

void loop() {
  Control_Surface.loop(); // Update the Control Surface
}