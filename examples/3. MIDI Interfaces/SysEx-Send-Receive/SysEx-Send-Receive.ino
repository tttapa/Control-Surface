/**
 * Example showing how to send and receive MIDI System Exclusive messages.
 * 
 * @boards  AVR, AVR USB, Nano Every, Due, Nano 33 IoT, Nano 33 BLE, Teensy 3.x, ESP32
 */

#include <Control_Surface.h>

// Instantiate the MIDI over USB interface
USBMIDI_Interface midi;

// Custom MIDI callback that prints incoming SysEx messages.
struct MyMIDI_Callbacks : MIDI_Callbacks {

  // This callback function is called when a SysEx message is received.
  void onSysExMessage(MIDI_Interface &, SysExMessage sysex) override {
    // Print the message
    Serial << F("Received SysEx message: ")         //
           << AH::HexDump(sysex.data, sysex.length) //
           << F(" on cable ") << sysex.cable.getOneBased() << endl;
  }

} callback {};

// Push button connected between pin 2 and ground.
// SysEx message is sent when pressed.
Button pushbutton {2};

void setup() {
  Serial.begin(115200);
  pushbutton.begin(); // enables internal pull-up
  midi.begin();
  midi.setCallbacks(callback);
}

void loop() {
  // Send a SysEx message when the push button is pressed
  uint8_t sysex[] {0xF0, 0x11, 0x22, 0x33, 0xF7};
  if (pushbutton.update() == Button::Falling)
    midi.sendSysEx(sysex);

  // Read incoming MIDI data and call the callback if a new
  // SysEx message has been received.
  midi.update();
}
