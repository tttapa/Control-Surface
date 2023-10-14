/** 
 * This is an example on how to attach your own callbacks for receiving MIDI 
 * input data.
 *
 * @boards  AVR, AVR USB, Nano Every, Due, Nano 33 IoT, Nano 33 BLE, UNO R4, Pi Pico, Teensy 3.x, ESP32, ESP8266
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
 * Written by PieterP, 2019-08-07  
 * https://github.com/tttapa/Control-Surface
 */

#include <Control_Surface.h> // Include the Control Surface library

USBMIDI_Interface midi;

bool channelMessageCallback(ChannelMessage cm) {
  Serial << F("Channel message: ") << hex                   //
         << cm.header << ' ' << cm.data1 << ' ' << cm.data2 //
         << dec << F(" on cable ") << cm.cable.getOneBased() << endl;
  return true; // Return true to indicate that handling is done,
               // and Control_Surface shouldn't handle it anymore.
               // If you want Control_Surface to handle it as well,
               // return false.
}

bool sysExMessageCallback(SysExMessage se) {
  Serial << F("System Exclusive message: [") << se.length << "] " //
         << AH::HexDump(se.data, se.length)                       //
         << F(" on cable ") << se.cable.getOneBased() << endl;
  return true; // Return true to indicate that handling is done,
               // and Control_Surface shouldn't handle it anymore.
               // If you want Control_Surface to handle it as well,
               // return false.
}

bool sysCommonMessageCallback(SysCommonMessage sc) {
  Serial << F("System Common message: ") << hex             //
         << sc.header << ' ' << sc.data1 << ' ' << sc.data2 //
         << dec << F(" on cable ") << sc.cable.getOneBased() << endl;
  return true; // Return true to indicate that handling is done,
               // and Control_Surface shouldn't handle it anymore.
               // If you want Control_Surface to handle it as well,
               // return false.
}

bool realTimeMessageCallback(RealTimeMessage rt) {
  Serial << F("Real-time message: ") //
         << hex << rt.message << dec //
         << F(" on cable ") << rt.cable.getOneBased() << endl;
  return true; // Return true to indicate that handling is done,
               // and Control_Surface shouldn't handle it anymore.
               // If you want Control_Surface to handle it as well,
               // return false.
}

void setup() {
  Serial.begin(115200);
  Control_Surface.begin();
  Control_Surface.setMIDIInputCallbacks(channelMessageCallback,   //
                                        sysExMessageCallback,     //
                                        sysCommonMessageCallback, //
                                        realTimeMessageCallback); //
  // If you don't need all four callbacks, you can pass `nullptr` instead of a
  // function pointer
}

void loop() {
  Control_Surface.loop();
}
