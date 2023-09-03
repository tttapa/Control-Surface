/**
 * This example demonstrates the use of push buttons and LEDs and how to use
 * shift registers and analog multiplexers to save pins.
 * 
 * @boards  AVR, AVR USB, Nano Every, Nano 33 IoT, Nano 33 BLE, UNO R4, Pi Pico, Due, Teensy 3.x, ESP8266, ESP32
 * 
 * Connections
 * -----------
 * 
 *   - SS:   74HC595 ST_CP
 *   - MOSI: 74HC595 DS
 *   - SCK:  74HC595 SH_CP
 *   - 2: 74HC4067 A (COM OUT/IN)
 *   - 3: 74HC4067 S0
 *   - 4: 74HC4067 S1
 *   - 5: 74HC4067 S2
 *   - 6: 74HC4067 S3
 * 
 * Connect 16 momentary push buttons between the input pins of the 
 * multiplexer and ground.  
 * The internal pull-up resistor for the buttons will be enabled automatically,
 * so no external resistors are necessary.
 * 
 * Connect 16 LEDs (and series resistors) between the eight outputs of the
 * two shift registers and ground. 
 * 
 * Remember to connect the enable pins of both the multiplexer and the shift
 * registers to ground in order to enable them. Also connect the master reset 
 * pin of the shift registers to Vcc.  
 * Connect the serial data output of the first shift register (QH' or Q7S) to
 * the serial input of the second shift register.
 * 
 * Behavior
 * --------
 *  
 * Pressing the first button will turn on the first LED. Pressing it again will
 * turn it off again.
 * Pressing the second button will turn on the second LED. Pressing it again 
 * will turn it off again, and so on.
 * 
 * Written by PieterP, 2018-08-28  
 * https://github.com/tttapa/Arduino-Helpers
 */

#include <Arduino_Helpers.h> // Include the Arduino Helpers library.

#include <AH/Containers/ArrayHelpers.hpp>
#include <AH/Hardware/Button.hpp>
#include <AH/Hardware/ExtendedInputOutput/AnalogMultiplex.hpp>
#include <AH/Hardware/ExtendedInputOutput/SPIShiftRegisterOut.hpp>

// Instantiate a multiplexer
CD74HC4067 mux {
  2,            // input pin
  {3, 4, 5, 6}, // Address pins S0, S1, S2, S3
  // 7, // Optionally, specify the enable pin
};

// Alternatively, if you have a 3-bit mux:
// CD74HC4051 mux {
//   2,
//   {3, 4, 5},
//   // 7, // Optional
// };

// Instantiate a shift register with the SPI slave select pin as latch pin, most
// significant bit first, and a total of 16 outputs.
SPIShiftRegisterOut<mux.length()> sreg {SPI, SS, MSBFIRST};

// Instantiate an array of momentary push buttons.
// It generates an array of Buttons on pins:
// { mux.pin(0), mux.pin(1) ... mux.pin(15) }
// For each button it creates, it increments the pin number by 1,
// and it starts counting from mux.pin(0)
auto buttons = generateIncrementalArray<Button, mux.length()>(mux.pin(0));

void setup() { // Initialize everything
  mux.begin();
  sreg.begin();
  for (Button &button : buttons)
    button.begin();
}

void loop() { // Check if a button is pressed, if so toggle the LED
  for (uint8_t i = 0; i < mux.length(); ++i)
    if (buttons[i].update() == Button::Falling)
      sreg.digitalWrite(i, sreg.digitalRead(i) ? LOW : HIGH);
}
