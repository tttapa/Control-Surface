/** 
 * This is an example on how to attach your own callbacks for receiving MIDI 
 * input.
 *
 * @boards  AVR, AVR USB, Nano Every, Due, Nano 33 IoT, Nano 33 BLE, Teensy 3.x, ESP32
 * 
 * Connections
 * -----------
 * 
 * None.
 * 
 * Behavior
 * --------
 * 
 * - When MIDI messages are sent to the USB interface, they are printed to the 
 *   Serial port.
 * 
 * Mapping
 * -------
 * 
 * None.
 * 
 * @see @ref MIDI-Input-Fine-Grained.ino
 * @see @ref MIDI-Input-Fine-Grained-All-Callbacks.ino
 *
 * Written by PieterP, 2020-06-11  
 * https://github.com/tttapa/Control-Surface
 */

#include <Control_Surface.h> // Include the Control Surface library

// The MIDI over USB interface to use
USBMIDI_Interface midi;

// Custom MIDI callback that prints incoming messages.
struct MyMIDI_Callbacks : MIDI_Callbacks {

  // Callback for channel messages (notes, control change, pitch bend, etc.).
  void onChannelMessage(MIDI_Interface &, ChannelMessage cm) override {
    // Print the message
    Serial << F("Channel message: ")                                        //
           << hex << cm.header << ' ' << cm.data1 << ' ' << cm.data2 << dec //
           << F(" on cable ") << cm.cable.getOneBased() << endl;
  }

  // Callback for system exclusive messages
  void onSysExMessage(MIDI_Interface &, SysExMessage se) override {
    // Print the message
    Serial << F("System Exclusive message: [") << se.length << "] " //
           << AH::HexDump(se.data, se.length)                       //
           << F(" on cable ") << se.cable.getOneBased() << endl;
  }

  // Callback for real-time messages
  void onRealTimeMessage(MIDI_Interface &, RealTimeMessage rt) override {
    // Print the message
    Serial << F("Real-time message: ") //
           << hex << rt.message << dec //
           << F(" on cable ") << rt.cable.getOneBased() << endl;
  }

} callback;

void setup() {
  Serial.begin(115200);        // For printing the messages
  midi.begin();                // Initialize the MIDI interface
  midi.setCallbacks(callback); // Attach the custom callback
}

void loop() {
  midi.update(); // Continuously handle MIDI input
}
