#include <AH/Containers/Array.hpp>
#include <Settings/NamespaceSettings.hpp>

#include "mbed/PluggableUSBMIDI.hpp"

#include <cstring> // memcpy

BEGIN_CS_NAMESPACE

struct USBDeviceMIDIBackend {
    using MIDIUSBPacket_t = AH::Array<uint8_t, 4>;
    MIDIUSBPacket_t read() { return u32_to_bytes(backend.read()); }
    void write(uint8_t cn_cin, uint8_t midi_0, uint8_t midi_1, uint8_t midi_2) {
        backend.write(bytes_to_u32(cn_cin, midi_0, midi_1, midi_2));
    }
    void sendNow() { backend.send_now(); }
    bool preferImmediateSend() { return false; }

    PluggableUSBMIDI backend;
};

END_CS_NAMESPACE