# ESP32-NimBLE

MIDI over BLE backend for the [Apache Mynewt NimBLE](https://github.com/apache/mynewt-nimble) library
on ESP32. Tested using the [h2zero/NimBLE-Arduino](https://github.com/h2zero/NimBLE-Arduino) library.

Ideally, all .ipp files would be .cpp files, and all .c.ipp files would be .c
files. However, since we want this component to be optional, it should not
compile anything unless the user explicitly includes any of the BLE-specific
headers. Otherwise, we would require every user to install the NimBLE-Arduino
library. Maybe we can clean this up later once the arduino-esp32 core ships
with NimBLE out of the box, or once Arduino comes up with a decent dependency
or library management system. In such a scenario, replace the inline functions
and variables with non-inline or even static functions/variables.
