#include "USBMIDI.hpp"
#include <MIDIUSB.h>

BEGIN_CS_NAMESPACE

namespace USBMIDI {

MIDIUSBPacket_t read() {
    midiEventPacket_t midipacket = MidiUSB.read();
    return MIDIUSBPacket_t{{midipacket.header, midipacket.byte1,
                            midipacket.byte2, midipacket.byte3}};
}

void write(uint8_t cn, uint8_t cin, uint8_t d0, uint8_t d1, uint8_t d2) {
    midiEventPacket_t msg = {
        uint8_t((cn << 4) | cin), // CN|CIN
        d0,                       // status
        d1,                       // data 1
        d2,                       // data 2
    };
    MidiUSB.sendMIDI(msg);
}

void flush() { MidiUSB.flush(); }

} // namespace USBMIDI

END_CS_NAMESPACE