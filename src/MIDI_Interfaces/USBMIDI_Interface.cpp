#include "USBMIDI_Interface.hpp"
#include <MIDI_Parsers/LambdaPuller.hpp>

// If the main MCU has a USB connection or is a Teensy with MIDI USB type
#if defined(USBCON) || defined(TEENSY_MIDIUSB_ENABLED) || !defined(ARDUINO)

BEGIN_CS_NAMESPACE

// -------------------------------------------------------------------------- //

// Reading MIDI

MIDIReadEvent USBMIDI_Interface::read() {
    auto pullpacket = [this] (MIDIUSBPacket_t &packet) {
        packet = readUSBPacket();
        return packet[0] != 0x00;
    };
    return parser.pull(LambdaPuller(std::move(pullpacket)));
}

void USBMIDI_Interface::update() {
    MIDIReadEvent event = read();
    while (event != MIDIReadEvent::NO_MESSAGE) { // As long as there are
                                                 // incoming messages
        dispatchMIDIEvent(event);
        event = read();
    }
    // TODO: check if we should block the pipe
}

bool USBMIDI_Interface::dispatchMIDIEvent(MIDIReadEvent event) {
    switch (event) {
        case MIDIReadEvent::NO_MESSAGE: return true; // LCOV_EXCL_LINE
        case MIDIReadEvent::CHANNEL_MESSAGE:
            return onChannelMessage(getChannelMessage());
        case MIDIReadEvent::SYSEX_CHUNK: // fallthrough
        case MIDIReadEvent::SYSEX_MESSAGE:
            return onSysExMessage(getSysExMessage());
        case MIDIReadEvent::REALTIME_MESSAGE:
            return onRealTimeMessage(getRealTimeMessage());
        case MIDIReadEvent::SYSCOMMON_MESSAGE: return true; // TODO
        default: return true;                               // LCOV_EXCL_LINE
    }
}

// -------------------------------------------------------------------------- //

// Sending MIDI

void USBMIDI_Interface::sendImpl(uint8_t header, uint8_t d1, uint8_t d2,
                                 Cable cn) {
    writeUSBPacket(cn, header >> 4, // CN|CIN
                   header,          // status
                   d1,              // data 1
                   d2);             // data 2
}

void USBMIDI_Interface::sendImpl(uint8_t header, uint8_t d1, Cable cn) {
    sendImpl(header, d1, 0, cn);
}

void USBMIDI_Interface::sendImpl(const uint8_t *data, size_t length,
                                 Cable cn) {
    while (length > 3) {
        writeUSBPacket(cn, 0x4, data[0], data[1], data[2]);
        data += 3;
        length -= 3;
    }
    switch (length) {
        case 3: writeUSBPacket(cn, 0x7, data[0], data[1], data[2]); break;
        case 2: writeUSBPacket(cn, 0x6, data[0], data[1], 0); break;
        case 1: writeUSBPacket(cn, 0x5, data[0], 0, 0); break;
        default: break;
    }
}

void USBMIDI_Interface::sendImpl(uint8_t rt, Cable cn) {
    writeUSBPacket(cn, 0xF, // CN|CIN
                   rt,      // single byte
                   0,       // no data
                   0);      // no data
}

END_CS_NAMESPACE

#endif
