/** 
 * This is an example on how to attach your own callbacks for receiving MIDI 
 * input. This example has fine-grained callbacks, i.e. one for each message 
 * type (Note On, Note Off, Control Change, System Exclusive ...).
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
 * @see @ref MIDI-Input.ino
 * @see @ref MIDI-Input-Fine-Grained-All-Callbacks.ino for a list of all 
 *           callbacks for all message types
 * 
 * Written by PieterP, 2021-06-26  
 * https://github.com/tttapa/Control-Surface
 */

#include <Control_Surface.h> // Include the Control Surface library

// The MIDI over USB interface to use
USBMIDI_Interface midi;

// Custom MIDI callback that prints incoming messages.
struct MyMIDI_Callbacks : FineGrainedMIDI_Callbacks<MyMIDI_Callbacks> {
  // Note how this ^ name is identical to the argument used here ^

  // Function that is called whenever a MIDI Note Off message is received.
  void onNoteOff(Channel channel, uint8_t note, uint8_t velocity, Cable cable) {
    Serial << "Note Off: " << channel << ", note " << note << ", velocity "
           << velocity << ", " << cable << endl;
  }

  // Function that is called whenever a MIDI Note On message is received.
  void onNoteOn(Channel channel, uint8_t note, uint8_t velocity, Cable cable) {
    Serial << "Note On: " << channel << ", note " << note << ", velocity "
           << velocity << ", " << cable << endl;
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
