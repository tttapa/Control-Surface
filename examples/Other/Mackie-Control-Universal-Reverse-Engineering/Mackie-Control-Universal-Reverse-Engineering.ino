/**
 * Sketch that reads the MIDI input from the USB MIDI Interface and prints it to
 * the Serial monitor, including the Mackie Control Universal note names.
 * 
 * This is useful to discover what kinds of messages your DAW is sending.
 * 
 * @boards  AVR, AVR USB, Nano Every, Due, Nano 33 IoT, Nano 33 BLE, UNO R4, Pi Pico, Teensy 3.x, ESP32, ESP8266
 */

#include <Control_Surface.h>

// A MIDI over USB interface
USBMIDI_Interface midi;

bool channelMessageCallback(ChannelMessage cm) {
  MIDIMessageType type = cm.getMessageType();
  if (type == MIDIMessageType::NoteOn || type == MIDIMessageType::NoteOff) {
    Serial << hex << cm.header << ' ' << cm.data1 << ' ' << cm.data2 << dec
           << "\t(" << MCU::getMCUNameFromNoteNumber(cm.data1) << ")"
           << F(" on cable ") << cm.cable.getOneBased() << endl;
  } else {
    Serial << hex << cm.header << ' ' << cm.data1 << ' ' << cm.data2 << dec
           << F(" on cable ") << cm.cable.getOneBased() << endl;
  }
  return false; // Return true to indicate that handling is done,
                // and Control_Surface shouldn't handle it anymore.
                // If you want Control_Surface to handle it as well,
                // return false.
}

bool sysExMessageCallback(SysExMessage se) {
  Serial << F("System Exclusive message: [") << se.length << "] " //
         << AH::HexDump(se.data, se.length)                       //
         << F(" on cable ") << se.cable.getOneBased() << endl;
  return false; // Return true to indicate that handling is done,
                // and Control_Surface shouldn't handle it anymore.
                // If you want Control_Surface to handle it as well,
                // return false.
}

bool sysCommonMessageCallback(SysCommonMessage sc) {
  Serial << F("System Common message: ") << hex << sc.getMessageType();
  if (sc.getNumberOfDataBytes() >= 1)
    Serial << sc.getData1();
  if (sc.getNumberOfDataBytes() >= 2)
    Serial << sc.getData2();
  Serial << dec << F(" on cable ") << sc.cable.getOneBased() << endl;
  return false; // Return true to indicate that handling is done,
                // and Control_Surface shouldn't handle it anymore.
                // If you want Control_Surface to handle it as well,
                // return false.
}

bool realTimeMessageCallback(RealTimeMessage rt) {
  Serial << F("Real-Time: ") << hex << rt.message << dec << F(" on cable ")
         << rt.cable.getOneBased() << endl;
  return false; // Return true to indicate that handling is done,
                // and Control_Surface shouldn't handle it anymore.
                // If you want Control_Surface to handle it as well,
                // return false.
}

void setup() {
  // Make sure that the Serial interface is fast enough, so it doesn't stall the
  // MIDI application
  Serial.begin(1000000);
  Control_Surface.begin();
  Control_Surface.setMIDIInputCallbacks(channelMessageCallback,   //
                                        sysExMessageCallback,     //
                                        sysCommonMessageCallback, //
                                        realTimeMessageCallback); //
}

void loop() {
  Control_Surface.loop();
}
