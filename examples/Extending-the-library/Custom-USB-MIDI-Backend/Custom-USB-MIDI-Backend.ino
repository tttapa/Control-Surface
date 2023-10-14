/**
 * This is an example that demonstrates how to extend the library using your own
 * MIDI USB backend. It defines a class for sending and receiving raw MIDI USB
 * packets that can be plugged into Control Surface's USBMIDI_Interface class.
 *
 * @see @ref src/MIDI_Interfaces/USBMIDI/USBMIDI_Teensy3.hpp
 * @see @ref src/MIDI_Interfaces/USBMIDI/USBMIDI_MIDIUSB.hpp
 * @see @ref src/MIDI_Interfaces/USBMIDI/USBMIDI_Adafruit_TinyUSB.hpp
 * @see @ref src/MIDI_Interfaces/USBHostMIDI_Interface.hpp
 *
 * @boards  AVR, AVR USB, Nano Every, Due, Nano 33 IoT, Nano 33 BLE, UNO R4, Pi Pico, Teensy 3.x, ESP32
 * 
 * Connections
 * -----------
 *
 * None.
 *
 * Behavior
 * --------
 *
 * - Prints the raw MIDI USB packets to the Serial Monitor.
 * - Receives no MIDI input.
 *
 * Mapping
 * -------
 *
 * None.
 * 
 * Written by PieterP, 2022-05-28  
 * https://github.com/tttapa/Control-Surface
 */
#include <Control_Surface.h>

// This class defines how to send and receive MIDI data using your custom
// backend.
struct MyUSBMIDIBackend {
  // USB MIDI packages are 4 bytes.
  using MIDIUSBPacket_t = AH::Array<uint8_t, 4>;
  // This method is optional. It is called once upon initialization.
  void begin() { Serial.begin(115200); }
  // Read a single packet. Return a packet of all zeros if there is no packet.
  MIDIUSBPacket_t read() { return MIDIUSBPacket_t {}; }
  // Write a single packet to the output buffer.
  void write(MIDIUSBPacket_t p) { Serial << AH::HexDump(p.data) << endl; }
  // Transmit the output buffer immediately.
  void sendNow() { Serial.flush(); }
  // Should the sendNow method be called after each message? Return false if
  // your backend can send the data in the output buffer asynchronously and
  // automatically. Return true if that's not the case, and an explicit call to
  // sendNow is required.
  bool preferImmediateSend() { return false; }
};

// This is the actual MIDI interface that makes use of the backend defined above.
struct MyUSBMIDI_Interface : GenericUSBMIDI_Interface<MyUSBMIDIBackend> {
  MyUSBMIDI_Interface() = default;
  using MIDIUSBPacket_t = MyUSBMIDIBackend::MIDIUSBPacket_t;
};

// Instantiate the MIDI interface to use.
MyUSBMIDI_Interface midi;

void setup() {
  Control_Surface.begin();
}

void loop() {
  static Timer<millis> timer {1000};
  if (timer)
    // As an example, send a MIDI timing clock message every second.
    Control_Surface.sendRealTime(MIDIMessageType::TimingClock);
  Control_Surface.loop();
}
