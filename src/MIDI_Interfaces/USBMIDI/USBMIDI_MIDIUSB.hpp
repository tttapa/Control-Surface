#include <AH/Containers/Array.hpp>
#include <Settings/NamespaceSettings.hpp>

#include <MIDIUSB.h>

BEGIN_CS_NAMESPACE

struct MIDIUSB_USBDeviceMIDIBackend {
    using MIDIUSBPacket_t = AH::Array<uint8_t, 4>;
    MIDIUSBPacket_t read() {
        midiEventPacket_t midipacket = MidiUSB.read();
        return {{midipacket.header, midipacket.byte1, midipacket.byte2,
                 midipacket.byte3}};
    }
    void write(MIDIUSBPacket_t d) {
        midiEventPacket_t msg {d.data[0], d.data[1], d.data[2], d.data[3]};
        MidiUSB.sendMIDI(msg);
    }
    void sendNow() { MidiUSB.flush(); }
    bool preferImmediateSend() { return true; }
};

END_CS_NAMESPACE