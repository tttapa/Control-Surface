#include "USBMIDI.hpp"

#include <AH/Teensy/TeensyUSBTypes.hpp>

BEGIN_CS_NAMESPACE

struct Teensy3_USBDeviceMIDIBackend {
    using MIDIUSBPacket_t = AH::Array<uint8_t, 4>;
    MIDIUSBPacket_t read();
    void write(MIDIUSBPacket_t data);
    void sendNow();
    bool preferImmediateSend();
};

END_CS_NAMESPACE

#ifdef TEENSY_MIDIUSB_ENABLED

#include <string.h>
#include <usb_midi.h>

BEGIN_CS_NAMESPACE

inline Teensy3_USBDeviceMIDIBackend::MIDIUSBPacket_t
Teensy3_USBDeviceMIDIBackend::read() {
    return u32_to_bytes(usb_midi_read_message());
}

inline void Teensy3_USBDeviceMIDIBackend::write(MIDIUSBPacket_t data) {
    usb_midi_write_packed(bytes_to_u32(data));
}

inline void Teensy3_USBDeviceMIDIBackend::sendNow() { usb_midi_flush_output(); }

inline bool Teensy3_USBDeviceMIDIBackend::preferImmediateSend() {
    return false;
}

END_CS_NAMESPACE

#endif