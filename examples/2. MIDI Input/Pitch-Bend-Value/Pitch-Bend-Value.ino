/**
 * This example demonstrates how to listen for incoming  MIDI Pitch Bend events.
 * 
 * @boards  AVR, AVR USB, Nano Every, Due, Nano 33 IoT, Nano 33 BLE, UNO R4, Pi Pico, Teensy 3.x, ESP32, ESP8266
 * 
 * Connections
 * -----------
 * 
 * None.
 * 
 * Behavior
 * --------
 * 
 * If a MIDI Pitch Bend message on channel 1 is received, its value is printed
 * to the serial port.
 * 
 * Mapping
 * -------
 * 
 * Route the MIDI output of a program or device that sends MIDI Pitch Bend 
 * messages to the Arduino's MIDI input. 
 * 
 * Written by PieterP, 2020-09-08  
 * https://github.com/tttapa/Control-Surface
 */
#include <Control_Surface.h>

USBMIDI_Interface midi;

// Instantiate an object that listens for MIDI Pitch Bend messages on channel 1.
PBValue pb {Channel_1};

void setup() {
  Control_Surface.begin();
  Serial.begin(115200);
}

void loop() {
  Control_Surface.loop();
  // If the Pitch Bend value changed
  if (pb.getDirty()) {
    // Print it
    Serial.println(pb.getValue());
    // Clear the dirty flag to acknowledge the change
    pb.clearDirty();
  }
}
