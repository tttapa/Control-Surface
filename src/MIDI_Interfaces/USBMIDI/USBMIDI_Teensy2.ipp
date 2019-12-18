#include "USBMIDI.hpp"
#include <AH/Teensy/TeensyUSBTypes.hpp>

#ifdef TEENSY_MIDIUSB_ENABLED

#include <usb_private.h>

BEGIN_CS_NAMESPACE

namespace USBMIDI {

MIDIUSBPacket_t read() {

    // https://github.com/PaulStoffregen/cores/blob/73ea157600a7082686d9cc48786a73caa7567da9/usb_midi/usb_api.cpp#L195
    uint8_t c, intr_state;

    intr_state = SREG;
    cli();
    if (!usb_configuration) {
        SREG = intr_state;
        return MIDIUSBPacket_t{0};
    }
    UENUM = MIDI_RX_ENDPOINT;
retry:
    c = UEINTX;
    if (!(c & (1 << RWAL))) {
        if (c & (1 << RXOUTI)) {
            UEINTX = 0x6B;
            goto retry;
        }
        SREG = intr_state;
        return MIDIUSBPacket_t{0};
    }
    MIDIUSBPacket_t packet;
    packet.data[0] = UEDATX;
    packet.data[1] = UEDATX;
    packet.data[2] = UEDATX;
    packet.data[3] = UEDATX;
    if (!(UEINTX & (1 << RWAL)))
        UEINTX = 0x6B;
    SREG = intr_state;

    return packet;
}

void write(uint8_t cn, uint8_t cin, uint8_t d0, uint8_t d1, uint8_t d2) {
    uint8_t intr_state, timeout;

    if (!usb_configuration)
        return;
    intr_state = SREG;
    cli();
    UENUM = MIDI_TX_ENDPOINT;
    timeout = UDFNUML + 2;
    while (1) {
        // are we ready to transmit?
        if (UEINTX & (1 << RWAL))
            break;
        SREG = intr_state;
        if (UDFNUML == timeout)
            return;
        if (!usb_configuration)
            return;
        intr_state = SREG;
        cli();
        UENUM = MIDI_TX_ENDPOINT;
    }
    UEDATX = (cn << 4) | cin;
    UEDATX = d0;
    UEDATX = d1;
    UEDATX = d2;
    if (!(UEINTX & (1 << RWAL)))
        UEINTX = 0x3A;
    SREG = intr_state;
}

void flush() {
    // TODO: I think the UEINTX = 0x3A actually sends/flushes the data, but I'm
    //       not sure, and I don't have the right hardware to test it.
}

} // namespace USBMIDI

END_CS_NAMESPACE

#endif