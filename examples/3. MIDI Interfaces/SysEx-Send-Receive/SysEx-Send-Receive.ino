/**
 * Example showing how to send and receive MIDI System Exclusive messages.
 * 
 * @boards  AVR, AVR USB, Nano Every, Due, Nano 33, Teensy 3.x, ESP32
 */

#include <Control_Surface.h>

// Instantiate the MIDI over USB interface
USBMIDI_Interface midi;

// Custom MIDI callback that prints incoming SysEx messages.
struct MyMIDI_Callbacks : MIDI_Callbacks {
 public:
  // This callback function is called when a SysEx message is received.
  void onSysExMessage(Parsing_MIDI_Interface &midi) override {
    // Get the message from the MIDI interface.
    SysExMessage sysex = midi.getSysExMessage();

    // Print the message
    Serial << F("Received SysEx message: ") << hex;
    for (uint8_t i = 0; i < sysex.length; ++i)
      Serial << sysex.data[i] << ' ';
    Serial << dec << F("\t on cable ") << sysex.CN << endl;
  }
} callback = {};

// Push button connected between pin 2 and ground.
// SysEx message is sent when pressed.
Button pushbutton = {2};

void setup() {
  Serial.begin(115200);
  pushbutton.begin(); // enables internal pull-up
  midi.begin();
  midi.setCallbacks(callback);
}

void loop() {
  // Send a SysEx message when the push button is pressed
  uint8_t sysex[] = {0xF0, 0x11, 0x22, 0x33, 0xF7};
  if (pushbutton.update() == Button::Falling)
    midi.send(sysex);

  // Read incoming MIDI data and call the callback if a new
  // SysEx message has been received.
  midi.update();
}
