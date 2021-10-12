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
 * @see @ref MIDI-Input-Fine-Grained.ino
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

  void onNoteOff(Channel channel, uint8_t note, uint8_t velocity, Cable cable) {
    Serial << "Note Off: " << channel << ", note " << note << ", velocity "
           << velocity << ", " << cable << endl;
  }
  void onNoteOn(Channel channel, uint8_t note, uint8_t velocity, Cable cable) {
    Serial << "Note On: " << channel << ", note " << note << ", velocity "
           << velocity << ", " << cable << endl;
  }
  void onKeyPressure(Channel channel, uint8_t note, uint8_t pressure,
                     Cable cable) {
    Serial << "Key Pressure: " << channel << ", note " << note << ", pressure "
           << pressure << ", " << cable << endl;
  }
  void onControlChange(Channel channel, uint8_t controller, uint8_t value,
                       Cable cable) {
    Serial << "Control Change: " << channel << ", controller " << controller
           << ", value " << value << ", " << cable << endl;
  }
  void onProgramChange(Channel channel, uint8_t program, Cable cable) {
    Serial << "Program Change: " << channel << ", program " << program << ", "
           << cable << endl;
  }
  void onChannelPressure(Channel channel, uint8_t pressure, Cable cable) {
    Serial << "Channel Pressure: " << channel << ", pressure " << pressure
           << ", " << cable << endl;
  }
  void onPitchBend(Channel channel, uint16_t bend, Cable cable) {
    Serial << "Pitch Bend: " << channel << ", bend " << bend << ", " << cable
           << endl;
  }
  void onSystemExclusive(SysExMessage se) {
    Serial << F("System Exclusive: [") << se.length << "] "
           << AH::HexDump(se.data, se.length) << ", " << se.cable << endl;
  }
  void onTimeCodeQuarterFrame(uint8_t data, Cable cable) {
    Serial << "MTC Quarter Frame: " << data << ", " << cable << endl;
  }
  void onSongPosition(uint16_t beats, Cable cable) {
    Serial << "Song Position Pointer: " << beats << ", " << cable << endl;
  }
  void onSongSelect(uint8_t songnumber, Cable cable) {
    Serial << "Song Select: " << songnumber << ", " << cable << endl;
  }
  void onTuneRequest(Cable cable) {
    Serial << "Tune Request: " << cable << endl;
  }
  void onClock(Cable cable) { Serial << "Timing Clock: " << cable << endl; }
  void onStart(Cable cable) { Serial << "Start: " << cable << endl; }
  void onContinue(Cable cable) { Serial << "Continue: " << cable << endl; }
  void onStop(Cable cable) { Serial << "Stop: " << cable << endl; }
  void onActiveSensing(Cable cable) {
    Serial << "Active Sensing: " << cable << endl;
  }
  void onSystemReset(Cable cable) {
    Serial << "System Reset: " << cable << endl;
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
