/**
 * MIDI routing using MIDI_Pipe%s.
 * 
 * Bridges the Bluetooth Low Energy MIDI interface to a Serial MIDI interface.
 * 
 * @boards  ESP32
 * 
 * Connections
 * -----------
 * 
 * - GPIO16 (RX2): a 5-pin DIN MIDI input
 * - GPIO17 (TX2): a 5-pin DIN MIDI output
 * 
 * For a detailed schematic, see 
 * <https://tttapa.github.io/Arduino/MIDI/Chap02-MIDI-Hardware.html>.  
 * Use the 3.3V version at the bottom of the page.
 * 
 * See the @ref MIDI_Routing module for more information about MIDI pipes.
 * 
 * Written by PieterP, 2020-02-04  
 * https://github.com/tttapa/Control-Surface
 */
#include <Control_Surface.h>

// Serial MIDI interface
HardwareSerialMIDI_Interface serialmidi = {Serial2, MIDI_BAUD};
// BLE MIDI interface
BluetoothMIDI_Interface blemidi;

// Pipe to connect the interfaces
BidirectionalMIDI_Pipe midipipe;

void setup() {
  // Connect the two MIDI interfaces using a bidirectional pipe
  serialmidi | midipipe | blemidi;

  // Initialize the MIDI interfaces
  serialmidi.begin();
  blemidi.begin();
}

void loop() {
  // Continuously handle input from the MIDI interfaces
  serialmidi.update();
  blemidi.update();
}
