#include <AH/Containers/Array.hpp>
#include <Settings/NamespaceSettings.hpp>

#include <gmock/gmock.h>

BEGIN_CS_NAMESPACE

struct USBDeviceMIDIBackend {
    using MIDIUSBPacket_t = AH::Array<uint8_t, 4>;

    MOCK_METHOD(void, write, (uint8_t, uint8_t, uint8_t, uint8_t));
    MOCK_METHOD(MIDIUSBPacket_t, read, ());
    MOCK_METHOD(void, sendNow, ());
    void write(MIDIUSBPacket_t d) {
        return write(d.data[0], d.data[1], d.data[2], d.data[3]);
    }
    static bool preferImmediateSend() { return false; }
};

END_CS_NAMESPACE