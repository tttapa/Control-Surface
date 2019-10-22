#include <Control_Surface.h> // Include the Control Surface library

USBDebugMIDI_Interface midi;

bool channelMessageCallback(ChannelMessage cm) {
    Serial << F("Channel message: ") << hex << cm.header << ' ' 
           << cm.data1 << ' ' << cm.data2 << dec
           << F(" on cable ") << cm.CN << endl;
    return true;  // Return true to indicate that handling is done, 
                  // and Control_Surface shouldn't handle it anymore.
                  // If you want Control_Surface to handle it as well, 
                  // return false.
}

bool sysExMessageCallback(SysExMessage se) {
    Serial << F("System Exclusive message: ") << hex;
    for (size_t i = 0; i < se.length; ++i)
      Serial << se.data[i] << ' ';
    Serial << dec << F("on cable ") << se.CN << endl;
    return true;  // Return true to indicate that handling is done, 
                  // and Control_Surface shouldn't handle it anymore.
                  // If you want Control_Surface to handle it as well, 
                  // return false.
}

bool realTimeMessageCallback(RealTimeMessage rt) {
    Serial << F("Real-Time message: ") << hex << rt.message << dec
           << F(" on cable ") << rt.CN << endl;
    return true;  // Return true to indicate that handling is done, 
                  // and Control_Surface shouldn't handle it anymore.
                  // If you want Control_Surface to handle it as well, 
                  // return false.
}

void setup() {
    Serial.begin(115200);
    Control_Surface.begin();
    Control_Surface.setMIDIInputCallbacks(channelMessageCallback, 
                                          sysExMessageCallback, 
                                          realTimeMessageCallback);
}

void loop() {
    Control_Surface.loop();
}
