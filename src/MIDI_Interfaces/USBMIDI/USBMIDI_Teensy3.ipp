#include "USBMIDI.hpp"

#include <AH/Teensy/TeensyUSBTypes.hpp>

#ifdef TEENSY_MIDIUSB_ENABLED

#include <string.h>
#include <usb_midi.h>

BEGIN_CS_NAMESPACE

namespace USBMIDI {

MIDIUSBPacket_t read() {
    MIDIUSBPacket_t packet;
    uint32_t msg = usb_midi_read_message();
    static_assert(sizeof(msg) == sizeof(packet.data), "incorrect packet size");
    memcpy(packet.data, &msg, sizeof(packet.data));
    return packet;
}

void write(uint8_t cn, uint8_t cin, uint8_t d0, uint8_t d1, uint8_t d2) {
    usb_midi_write_packed((cn << 4) | cin | // CN|CIN
                          (d0 << 8) |       // status
                          (d1 << 16) |      // data 1
                          (d2 << 24));      // data 2
}

void flush() {}

} // namespace USBMIDI

END_CS_NAMESPACE

#endif