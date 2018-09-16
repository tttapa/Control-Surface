#include <Control_Surface.h>

USBMIDI_Interface midi;

Button enableButton = 2; // Momentary push button between pin 2 and ground
PBPotentiometer masterVolume = { A0, 9 }; // Potentiometer to pin A0

void setup() {
  enableButton.begin();
  Control_Surface.begin();
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
}

void loop() {
  static bool state = true;
  if (enableButton.getState() == Button::Falling) {
    state = !state;
    digitalWrite(LED_BUILTIN, state);
    state ? masterVolume.enable() : masterVolume.disable();
  }
  Control_Surface.loop();
}