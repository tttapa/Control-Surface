/**
 * This is the example used in the 'Getting Started' guide.  
 * Read the guide: https://github.com/tttapa/Control-Surface/blob/revision-4/doc/Getting-Started.md
 * 
 * Written by PieterP, 29-08-2018  
 * https://github.com/tttapa/Control-Surface
 */

#include <Control_Surface.h>

USBMIDI_Interface midi;

CD74HC4051 mux = { A0, {10, 11, 12} };

CCPotentiometer volumePotentiometers[] = {
    { mux.pin(0), {MIDI_CC::Channel_Volume, CHANNEL_1} },
    { mux.pin(1), {MIDI_CC::Channel_Volume, CHANNEL_2} },
    { mux.pin(2), {MIDI_CC::Channel_Volume, CHANNEL_3} },
    { mux.pin(3), {MIDI_CC::Channel_Volume, CHANNEL_4} },
    { mux.pin(4), {MIDI_CC::Channel_Volume, CHANNEL_5} },
    { mux.pin(5), {MIDI_CC::Channel_Volume, CHANNEL_6} },
    { mux.pin(6), {MIDI_CC::Channel_Volume, CHANNEL_7} },
    { mux.pin(7), {MIDI_CC::Channel_Volume, CHANNEL_8} },
};

void setup() {
    Control_Surface.begin();
}

void loop() {
    Control_Surface.loop();
}
