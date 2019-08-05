#include <Control_Surface.h> // Include the Control Surface library

USBDebugMIDI_Interface midi;

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
    Control_Surface.setMIDIInputCallbacks(nullptr, 
                                          nullptr, 
                                          realTimeMessageCallback);
}

void loop() {
    Control_Surface.loop();
}
