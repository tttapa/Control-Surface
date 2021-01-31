#include <AH/Teensy/TeensyUSBTypes.hpp>

#include <AH/Containers/Array.hpp>
#include <Settings/NamespaceSettings.hpp>

BEGIN_CS_NAMESPACE

struct USBDeviceMIDIBackend {
    using MIDIUSBPacket_t = AH::Array<uint8_t, 4>;
    MIDIUSBPacket_t read();
    void write(uint8_t cn, uint8_t cin, uint8_t d0, uint8_t d1, uint8_t d2);
    void sendNow();
    bool preferImmediateSend();
};

END_CS_NAMESPACE

#ifdef TEENSY_MIDIUSB_ENABLED

#include <string.h>
#include <usb_midi.h>

BEGIN_CS_NAMESPACE

inline USBDeviceMIDIBackend::MIDIUSBPacket_t USBDeviceMIDIBackend::read() {
    MIDIUSBPacket_t packet;
    uint32_t msg = usb_midi_read_message();
    static_assert(sizeof(msg) == sizeof(packet.data), "incorrect packet size");
    memcpy(packet.data, &msg, sizeof(packet.data));
    return packet;
}

inline void USBDeviceMIDIBackend::write(uint8_t cn, uint8_t cin, uint8_t d0,
                                        uint8_t d1, uint8_t d2) {
    usb_midi_write_packed((cn << 4) | cin | // CN|CIN
                          (d0 << 8) |       // status
                          (d1 << 16) |      // data 1
                          (d2 << 24));      // data 2
}

inline void USBDeviceMIDIBackend::sendNow() { usb_midi_flush_output(); }

inline bool USBDeviceMIDIBackend::preferImmediateSend() { return false; }

END_CS_NAMESPACE

#endif