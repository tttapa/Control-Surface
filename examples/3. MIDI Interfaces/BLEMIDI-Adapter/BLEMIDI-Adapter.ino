/**
 * Turns an Arduino into a Bluetooth Low Energy (BLE) to 5-pin DIN MIDI adapter.
 * 
 * @boards  Nano 33 IoT, Nano 33 BLE, ESP32, ESP32-S3, Pi Pico W
 *
 * Configuration
 * -------------
 *
 * - If you're using a Pi Pico W, you'll have to enable the Bluetooth stack
 *     from the _Tools > IP/Bluetooth Stack_ menu in the IDE.
 * - If you're using an ESP32, you can optionally switch to the NimBLE backend
 *     by installing the [h2zero/NimBLE-Arduino](https://github.com/h2zero/NimBLE-Arduino)
 *     library, and adding `#define CS_USE_NIMBLE` at the top of this sketch.
 * - If you're not using a Pico or an ESP32, you'll have to install the
 *     [ArduinoBLE](https://github.com/arduino-libraries/ArduinoBLE) library.
 *
 * Connections
 * -----------
 * 
 * - TXD: connected to a MIDI 5-pin DIN output connector
 *     (with series resistor, possibly through a buffer)
 * - RXD: connected to a MIDI 5-pin DIN input connector
 *     (with an optocoupler)
 *
 * See https://midi.org/specifications/midi-transports-specifications/5-pin-din-electrical-specs
 * for the schematic, optocoupler models and resistor values.
 *
 * Behavior
 * --------
 *
 * - The Arduino will advertise itself as a Bluetooth Low Energy MIDI device
 *     with the name `MIDI Adapter`.
 * - When you connect to the Arduino using your phone or computer, the built-in
 *     LED turns on to indicate that the connection was successful.
 * - Any MIDI messages sent to the Arduino over BLE are sent out to the 5-pin
 *     DIN output connector.
 * - Any MIDI messages sent to the Arduino through the 5-pin DIN input connector
 *     are sent over BLE.
 *
 * @see @ref md_pages_MIDI-over-BLE
 * @see @ref midi-tutorial
 * 
 * Written by PieterP, 2024-01-21  
 * https://github.com/tttapa/Control-Surface
 */

#include <Control_Surface.h>
#include <MIDI_Interfaces/BluetoothMIDI_Interface.hpp>

// Instantiate a MIDI over BLE interface
BluetoothMIDI_Interface midi_ble;
// Instantiate a 5-pin DIN MIDI interface (on the TX and RX pins of Serial1)
HardwareSerialMIDI_Interface midi_ser {Serial1};
// Instantiate the pipe to connect the two interfaces
BidirectionalMIDI_Pipe pipes;

void setup() {
  // Change the name of the BLE device (must be done before initializing it)
  midi_ble.setName("MIDI Adapter");
  // Manually route MIDI input from the serial interface to the BLE interface,
  // and the MIDI input from the BLE interface to the serial interface
  midi_ser | pipes | midi_ble;
  // Initialize the MIDI interfaces
  MIDI_Interface::beginAll();
  // Initialize the built-in LED
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  // Continuously poll all interfaces and route the traffic between them
  MIDI_Interface::updateAll();
  // Display the connection status using the built-in LED
  digitalWrite(LED_BUILTIN, midi_ble.isConnected() ? HIGH : LOW);
}
