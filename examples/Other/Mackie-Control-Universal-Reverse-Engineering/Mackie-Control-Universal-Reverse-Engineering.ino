/**
 * Sketch that reads the MIDI input from the USB MIDI Interface and prints it to
 * the Serial monitor, including the Mackie Control Universal note names.
 * 
 * This is useful to discover what kinds of messages your DAW is sending.
 * 
 * @boards  AVR, AVR USB, Due, Nano 33, Teensy 3.x, ESP32
 */


#include <Control_Surface.h>

// A MIDI over USB interface
USBMIDI_Interface midi;

bool channelMessageCallback(ChannelMessage cm) {
  if ((cm.header & 0xF0) == 0x90)
    Serial << hex << cm.header << ' ' << cm.data1 << ' ' << cm.data2 << dec 
           << "\t(" <<  MCU::getMCUNameFromNoteNumber(cm.data1)  << ")" << endl;
  else 
    Serial << hex << cm.header << ' ' << cm.data1 << ' ' << cm.data2 << dec << endl;
  return false; // Return true to indicate that handling is done,
                // and Control_Surface shouldn't handle it anymore.
                // If you want Control_Surface to handle it as well,
                // return false.
}
 
bool sysExMessageCallback(SysExMessage se) {
  Serial << F("SysEx: ") << hex;
  for (size_t i = 0; i < se.length; ++i)
    Serial << se.data[i] << ' ';
  Serial << dec << F("on cable ") << se.CN << endl;
  return false; // Return true to indicate that handling is done,
                // and Control_Surface shouldn't handle it anymore.
                // If you want Control_Surface to handle it as well,
                // return false.
}
 
bool realTimeMessageCallback(RealTimeMessage rt) {
  Serial << F("Real-Time: ") << hex << rt.message << dec
         << F(" on cable ") << rt.CN << endl;
  return false; // Return true to indicate that handling is done,
                // and Control_Surface shouldn't handle it anymore.
                // If you want Control_Surface to handle it as well,
                // return false.
}

void setup() {
  // Make sure that the Serial interface is fast enough, so it doesn't stall the MIDI 
  // application
  Serial.begin(1000000);
  Control_Surface.begin();
  Control_Surface.setMIDIInputCallbacks(channelMessageCallback,   //
                                        sysExMessageCallback,     //
                                        realTimeMessageCallback); //
}

void loop() {
  Control_Surface.loop();
}
