#include <MIDIUSB.h>

#include <AH/Containers/Array.hpp>
#include <Settings/NamespaceSettings.hpp>

BEGIN_CS_NAMESPACE

struct USBDeviceMIDIBackend {
    using MIDIUSBPacket_t = AH::Array<uint8_t, 4>;
    MIDIUSBPacket_t read() {
        midiEventPacket_t midipacket = MidiUSB.read();
        return MIDIUSBPacket_t{{midipacket.header, midipacket.byte1,
                                midipacket.byte2, midipacket.byte3}};
    }
    void write(uint8_t cn, uint8_t cin, uint8_t d0, uint8_t d1, uint8_t d2) {
        midiEventPacket_t msg{uint8_t((cn << 4) | cin), d0, d1, d2};
        MidiUSB.sendMIDI(msg);
    }
    void sendNow() { MidiUSB.flush(); }
    bool preferImmediateSend() { return true; }
};

END_CS_NAMESPACE