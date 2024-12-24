/**
 * @brief   This is an example of the `CCPotentiometer` class of the 
 *          Control_Surface library, and specifically, how to use the `map`
 *          function to account for non-linearities of the potentiometer.
 * 
 * Connect a potentiometer to analog pin A0. This will be the MIDI channel 
 * volume of channel 1.  
 * Map it in your DAW or DJ software.
 * 
 * Written by Pieter P, 16-06-2019  
 * https://github.com/tttapa/Control-Surface
 */

// Include the Control Surface library
#include <Control_Surface.h>

// Instantiate a MIDI over USB interface.
USBMIDI_Interface midi;

// Create a new instance of the class `CCPotentiometer`, called `potentiometer`,
// on pin A0, that sends MIDI messages with controller 7 (channel volume)
// on channel 1.
CCPotentiometer potentiometer(A0, {MIDI_CC::Channel_Volume, CHANNEL_1});

// The filtered value read when potentiometer is at the 0% position
constexpr int minimumValue = 2;
// The filtered value read when potentiometer is at the 100% position
constexpr int maximumValue = 125;

// A mapping function to eliminate the dead zones of the potentiometer:
// Some potentiometers don't output a perfect zero signal when you move them to
// the zero position, they will still output a value of 1 or 2, and the same
// goes for the maximum position.
uint8_t mappingFunction(uint8_t raw) {
    // make sure that the analog value is between the minimum and maximum
    raw = constrain(raw, minimumValue, maximumValue);
    // map the value from [minimumValue, maximumValue] to [0, 127]
    return map(raw, minimumValue, maximumValue, 0, 127);
}

void setup() {
    // Add the mapping function to the potentiometer
    potentiometer.map(mappingFunction);
    Serial.begin(115200);
    // Initialize everything
    Control_Surface.begin();
}

void loop() {
    // Update the Control Surface (check whether the potentiometer's
    // input has changed since last time, if so, send the new value over MIDI).
    Control_Surface.loop();
    // Use this to find minimumValue and maximumValue: it prints the raw value
    // of the potentiometer, without the mapping function
    Serial.println(potentiometer.getRawValue());
}