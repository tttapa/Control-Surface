#include <Control_Surface.h>

#include "MIDI-challenge.h"

template <class T, size_t N>
constexpr size_t len(T (&)[N]) {
    return N;
}

USBMIDI_Interface midi;

void setup() {
    Serial.begin(115200);
    midi.begin();
}

void loop() {
    if (Serial.read() >= 0) {
        delay(100);
        for (size_t i = 0; i < len(messages); ++i) {
            midi.backend.write(u32_to_bytes(pgm_read_dword(messages + i)));
        }
    }
}