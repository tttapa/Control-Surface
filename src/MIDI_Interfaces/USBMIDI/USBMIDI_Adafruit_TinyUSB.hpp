#include <AH/Containers/Array.hpp>
#include <Settings/NamespaceSettings.hpp>

#include <Adafruit_TinyUSB.h>

BEGIN_CS_NAMESPACE

struct Adafruit_TinyUSB_USBDeviceMIDIBackend {
    using MIDIUSBPacket_t = AH::Array<uint8_t, 4>;
    void begin() {
        // Manual begin() is required on core without built-in support e.g. mbed rp2040
        if (!TinyUSBDevice.isInitialized())
            TinyUSBDevice.begin(0);
        backend.begin();
        // If already enumerated, additional class driver begin() e.g msc, hid, midi won't take effect until re-enumeration
        if (TinyUSBDevice.mounted()) {
            TinyUSBDevice.detach();
            delay(10);
            TinyUSBDevice.attach();
        }
    }
    MIDIUSBPacket_t read() {
#ifdef TINYUSB_NEED_POLLING_TASK
        TinyUSBDevice.task();
#endif
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