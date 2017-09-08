#include <MIDI_Controller.h> // include the library

const size_t analogAverage = 8; // Use the average of 8 samples to get smooth transitions and prevent noise
const uint8_t channel = 1; // MIDI channel 1

Analog potentiometers[] = { // create an array of Analog objects
  {A0, 60, channel}, // a potentiometer connected to pin A0, sends MIDI CC events on controller number 60, on channel 1
  {A1, 61, channel}
};
const size_t nb_potentiometers = sizeof(potentiometers) / sizeof(Analog);

void setup(){
  USBMidiController.blink(LED_BUILTIN);  // flash the built-in LED (pin 13 on most boards) on every message
  USBMidiController.setDelay(15);  // wait 15 ms after each message not to flood the connection
  USBMidiController.begin(115200);  // Initialise the USB MIDI connection

  for(int i = 0; i < nb_potentiometers; i++) {
    potentiometers[i].average(analogAverage); // Use the average of 8 samples to get smooth transitions and prevent noise
  }
}

void loop(){
  for(int i = 0; i < nb_potentiometers; i++) { // refresh all Analog inputs
    potentiometers[i].refresh();
  }
}
