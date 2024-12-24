#include <Control_Surface.h>

USBMIDI_Interface midi;

Button enableButton = 2; // Momentary push button between pin 2 and ground
PBPotentiometer masterVolume = { A0, CHANNEL_9 }; // Potentiometer to pin A0

void setup() {
  enableButton.begin();
  Control_Surface.begin();
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
}

void loop() {
  if (enableButton.getState() == Button::Falling) {
    static bool state = true;
    state = !state;
    digitalWrite(LED_BUILTIN, state);
    state ? masterVolume.enable() : masterVolume.disable();
  }
  Control_Surface.loop();
}