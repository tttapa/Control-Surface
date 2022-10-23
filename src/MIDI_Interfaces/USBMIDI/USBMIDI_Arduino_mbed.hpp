#include <AH/Containers/Array.hpp>
#include <Settings/NamespaceSettings.hpp>

#include "mbed/PluggableUSBMIDI.hpp"

BEGIN_CS_NAMESPACE

struct Arduino_mbed_USBDeviceMIDIBackend {
    using MIDIUSBPacket_t = AH::Array<uint8_t, 4>;
    MIDIUSBPacket_t read() { return u32_to_bytes(backend.read()); }
    void write(MIDIUSBPacket_t data) { backend.write(bytes_to_u32(data)); }
    void sendNow() { backend.send_now(); }
    bool preferImmediateSend() { return false; }

    PluggableUSBMIDI backend;
};

END_CS_NAMESPACE