#include <Control_Surface.h>

USBMIDI_Interface midi;

void setup() {
    midi.begin();
    midi.backend.backend.setTimeout(std::chrono::microseconds{1'000'000});
}

void loop() {
    auto msg = midi.backend.read();
    if (msg.data[0] != 0) {
        midi.backend.write(msg);
        // delayMicroseconds(1000);
    }
}