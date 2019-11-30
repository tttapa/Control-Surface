#include <Control_Surface.h> // Include the Control Surface library

USBMIDI_Interface midi;

// Return true to indicate that handling is done, and Control_Surface shouldn't
// handle it anymore. If you want Control_Surface to handle it as well, return
// false.
bool channelMsgCallback(ChannelMessage cm) {
  ChannelMessageMatcher matcher = cm;
  // Filter the channels if necessary
  if (Channel(matcher.channel) == CHANNEL_1) {
    // Extract note data
    uint8_t note = cm.data1;
    uint8_t velocity = cm.data2;

    if (matcher.type == NOTE_ON) {

      // Play tone here

      return true;
    } else if (matcher.type == NOTE_OFF) {

      // Stop playing tone here

      return true;
    }
  }
  return false;
}

void setup() {
  Serial.begin(115200);
  Control_Surface.begin();
  Control_Surface.setMIDIInputCallbacks(channelMsgCallback, nullptr, nullptr);
}

void loop() {
  Control_Surface.loop();
}
