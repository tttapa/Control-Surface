/**
 * This examples shows how to use a button matrix to read many switches using
 * a BD to decimal decoder.  
 * https://forum.arduino.cc/index.php?topic=651047
 * 
 * @boards  AVR, AVR USB, Nano Every, Due, Nano 33 IoT, Nano 33 BLE, UNO R4, Pi Pico, Teensy 3.x, ESP32, ESP8266
 * 
 * Written by Pieter P, 2019-12-01  
 * https://github.com/tttapa/Control-Surface
 */

#include <Control_Surface.h>

#include <AH/STL/bitset>

// Pick one MIDI interface to use:
USBDebugMIDI_Interface midi(115200); // baud rate, use for easy debugging
// USBMIDI_Interface midi; // Use for final sketch with MIDI firmware
// HairlessMIDI_Interface midi; // Use for Hairless Serial <-> MIDI bridge

// Dimensions of the keyboard matrix
constexpr uint8_t rows = 10;
constexpr uint8_t columns = 8;

static_assert(rows <= 10, "Error: TC4028 only has 10 outputs");

// The note numbers corresponding to the buttons in the matrix
const AddressMatrix<rows, columns> notes {{
  {1, 2, 3, 4, 5, 6, 7, 8},        //
  {9, 10, 11, 12, 13, 14, 15, 16}, //
  // 8 more rows ...
}};
// The MIDI channel to use
const Channel channel = Channel_1;

// Address pins A, B, C & D of TC4028
const AH::PinList<4> addressPins {{2, 3, 4, 5}};
// The pins connected to the columns of the keyboard matrix (with external
// pull-down resistors)
const AH::PinList<columns> columnPins {{6, 7, 8, 9, 10, 11, 12, 13}};

// Make the given line [0, 9] of the TC4028 high, the nine others low
void selectLine(uint8_t line) {
  uint8_t mask = 1;
  for (pin_t addressPin : addressPins) {
    digitalWrite(addressPin, (line & mask) != 0 ? HIGH : LOW);
    mask <<= 1;
  }
}

void setup() {
  for (pin_t addressPin : addressPins)
    pinMode(addressPin, OUTPUT);
  for (pin_t columnPin : columnPins)
    pinMode(columnPin, INPUT);

  midi.begin();
}

// Array of bits to remember previous switch states for state change detection.
std::bitset<rows * columns> previousStates;

void loop() {
  for (uint8_t row = 0; row < rows; ++row) {
    selectLine(row);
    for (uint8_t col = 0; col < columns; ++col) {
      bool newState = digitalRead(columnPins[col]);
      uint8_t index = row * columns + col; // index in the bit array
      // If the state of the switch changed
      if (newState != previousStates[index]) {
        // Send a MIDI note message
        // (note on message with zero velocity == note off)
        uint8_t velocity = newState * 0x7F;
        midi.sendNoteOn({notes[row][col], channel}, velocity);
        // Remember this new state
        previousStates[index] = newState;
        // TODO: add debouncing
      }
    }
  }
  midi.update(); // read and discard MIDI input
}