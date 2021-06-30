#include <AH/Containers/Array.hpp>
#include <Settings/NamespaceSettings.hpp>

#include <MIDIUSB.h>

BEGIN_CS_NAMESPACE

struct USBDeviceMIDIBackend {
    using MIDIUSBPacket_t = AH::Array<uint8_t, 4>;
    MIDIUSBPacket_t read() {
        midiEventPacket_t midipacket = MidiUSB.read();
        return {{midipacket.header, midipacket.byte1, midipacket.byte2,
                 midipacket.byte3}};
    }
    void write(uint8_t cn_cin, uint8_t midi_0, uint8_t midi_1, uint8_t midi_2) {
        midiEventPacket_t msg{cn_cin, midi_0, midi_1, midi_2};
        MidiUSB.sendMIDI(msg);
    }
    void sendNow() { MidiUSB.flush(); }
    bool preferImmediateSend() { return true; }
};

END_CS_NAMESPACE