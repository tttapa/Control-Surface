#include <Control_Surface.h>

#include "MIDI-challenge.h"

template <class T, size_t N>
constexpr size_t len(T (&)[N]) {
    return N;
}

USBMIDI_Interface midi;

void setup() {
    Serial.begin(115200);
    Serial1.begin(115200);
    midi.begin();
    pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
    static size_t index = 0;
    static bool fail = false;
    if (Serial.read() >= 0) {
        digitalWrite(LED_BUILTIN, LOW);
        index = 0;
        fail = false;
    }
    uint32_t msg = bytes_to_u32(midi.backend.read());
    if (msg != 0) {
        uint32_t exp = pgm_read_dword(messages + index);
        if (exp != msg) {
            fail = true;
            digitalWrite(LED_BUILTIN, HIGH);
        }
        ++index;
        if (index == len(messages)) {
            index = 0;
            Serial.println(fail ? '1' : '0');
            Serial1.println(fail ? '1' : '0');
        }
    }
}