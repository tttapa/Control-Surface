#include <AH/Containers/Array.hpp>
#include <Settings/NamespaceSettings.hpp>

#include <Adafruit_TinyUSB.h>

BEGIN_CS_NAMESPACE

struct Adafruit_TinyUSB_USBDeviceMIDIBackend {
    using MIDIUSBPacket_t = AH::Array<uint8_t, 4>;
    void begin() {
#if defined(ARDUINO_ARCH_MBED) && defined(ARDUINO_ARCH_RP2040)
        // Manual begin() is required on core without built-in support for TinyUSB such as mbed rp2040
        TinyUSB_Device_Init(0);
#endif
        backend.begin();
    }
    MIDIUSBPacket_t read() {
        MIDIUSBPacket_t packet {};
        if (TinyUSBDevice.mounted())
            backend.readPacket(packet.data);
        return packet;
    }
    void write(MIDIUSBPacket_t packet) {
        if (TinyUSBDevice.mounted())
            backend.writePacket(packet.data);
    }
    void sendNow() {
        if (TinyUSBDevice.mounted())
            backend.flush();
    }
    bool preferImmediateSend() { return false; }

    Adafruit_USBD_MIDI backend;
};

END_CS_NAMESPACE