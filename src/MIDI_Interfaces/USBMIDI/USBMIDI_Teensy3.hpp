#include "USBMIDI.hpp"

#include <AH/Teensy/TeensyUSBTypes.hpp>

BEGIN_CS_NAMESPACE

struct USBDeviceMIDIBackend {
    using MIDIUSBPacket_t = AH::Array<uint8_t, 4>;
    MIDIUSBPacket_t read();
    void write(uint8_t cn_cin, uint8_t midi_0, uint8_t midi_1, uint8_t midi_2);
    void sendNow();
    bool preferImmediateSend();
};

END_CS_NAMESPACE

#ifdef TEENSY_MIDIUSB_ENABLED

#include <string.h>
#include <usb_midi.h>

BEGIN_CS_NAMESPACE

inline USBDeviceMIDIBackend::MIDIUSBPacket_t USBDeviceMIDIBackend::read() {
    return u32_to_bytes(usb_midi_read_message());
}

inline void USBDeviceMIDIBackend::write(uint8_t cn_cin, uint8_t midi_0,
                                        uint8_t midi_1, uint8_t midi_2) {
    usb_midi_write_packed(bytes_to_u32(cn_cin, midi_0, midi_1, midi_2));
}

inline void USBDeviceMIDIBackend::sendNow() { usb_midi_flush_output(); }

inline bool USBDeviceMIDIBackend::preferImmediateSend() { return false; }

END_CS_NAMESPACE

#endif