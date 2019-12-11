/**
 * <https://forum.arduino.cc/index.php?topic=653004>
 * 
 * Listen to MIDI notes 0 and 1, and print when Note On events are received.  
 * This was initially intended to send IR remote commands when a note event
 * is received for one of these notes.
 */

#include <Control_Surface.h>

USBMIDI_Interface midi; // MIDI Interface to use

void sendIR(uint32_t cmd) {
  Serial << "Send IR 0x" << hex << cmd << dec << endl;
  // Implement this
}

struct MyCallback final : SimpleNoteCCValueCallback {
  // Function is called when note event for given range of notes is received.
  void update(const INoteCCValue &noteInput, uint8_t index) override {
    if (noteInput.getValue(index) == 0) // check that velocity > 0
      return;
    switch (index) { // index in the range of notes
      case 0: sendIR(0xFFE01F); break;
      case 1: sendIR(0xFF609F); break;
    }
  }
};

// Listen to a range of 2 notes, and use MyCallback.
GenericNoteRange<2, MyCallback> noteInput = {
  0,            // first note number
  MyCallback(), // callback to use
};

void setup() {
  Serial.begin(115200);
  Control_Surface.begin();
}

void loop() {
  Control_Surface.loop();
}
