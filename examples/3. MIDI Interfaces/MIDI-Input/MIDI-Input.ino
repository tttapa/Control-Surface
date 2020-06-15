/** 
 * This is an example on how to attach your own callbacks for receiving MIDI 
 * input.
 *
 * @boards  AVR, AVR USB, Nano Every, Due, Nano 33, Teensy 3.x, ESP32
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
 * Written by PieterP, 2020-06-11  
 * https://github.com/tttapa/Control-Surface
 */

#include <Control_Surface.h> // Include the Control Surface library

// The MIDI over USB interface to use
USBMIDI_Interface midi;

// Custom MIDI callback that prints incoming messages.
struct MyMIDI_Callbacks : MIDI_Callbacks {

  // Callback for channel messages (notes, control change, pitch bend, etc.).
  void onChannelMessage(Parsing_MIDI_Interface &midi) override {
    ChannelMessage cm = midi.getChannelMessage();
    // Print the message
    Serial << F("Channel message: ") << hex << cm.header << ' ' << cm.data1
           << ' ' << cm.data2 << dec << F(" on cable ") << cm.CN << endl;
  }

  // Callback for system exclusive messages
  void onSysExMessage(Parsing_MIDI_Interface &midi) override {
    SysExMessage se = midi.getSysExMessage();
    // Print the message
    Serial << F("System Exclusive message: ") << hex;
    for (size_t i = 0; i < se.length; ++i)
      Serial << se.data[i] << ' ';
    Serial << dec << F("on cable ") << se.CN << endl;
  }

  // Callback for real-time messages
  void onRealTimeMessage(Parsing_MIDI_Interface &midi) override {
    RealTimeMessage rt = midi.getRealTimeMessage();
    // Print the message
    Serial << F("Real-time message: ") << hex << rt.message << dec
           << F(" on cable ") << rt.CN << endl;
  }

} callbacks;

void setup() {
  Serial.begin(115200);         // For printing the messages
  midi.begin();                 // Initialize the MIDI interface
  midi.setCallbacks(callbacks); // Attach the custom callback
}

void loop() {
  midi.update(); // Continuously handle MIDI input
}
