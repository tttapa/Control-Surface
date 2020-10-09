#include "USBMIDI_Interface.hpp"

// If the main MCU has a USB connection or is a Teensy with MIDI USB type
#if defined(USBCON) || defined(TEENSY_MIDIUSB_ENABLED) || !defined(ARDUINO)

BEGIN_CS_NAMESPACE

// -------------------------------------------------------------------------- //

// Reading MIDI

MIDIReadEvent USBMIDI_Interface::read() {
    for (uint8_t i = 0; i < (SYSEX_BUFFER_SIZE + 2) / 3; ++i) {
        MIDIUSBPacket_t midi_packet = readUSBPacket();
        if (midi_packet.data[0] == 0)
            return MIDIReadEvent::NO_MESSAGE;

        MIDIReadEvent parseResult = parser.parse(midi_packet.data);

        if (parseResult != MIDIReadEvent::NO_MESSAGE)
            return parseResult;
    }
    return MIDIReadEvent::NO_MESSAGE;
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
        case MIDIReadEvent::NO_MESSAGE: 
            return true;
        case MIDIReadEvent::CHANNEL_MESSAGE:
            return onChannelMessage(getChannelMessage());
        case MIDIReadEvent::SYSEX_MESSAGE: 
            return onSysExMessage(getSysExMessage());
        case MIDIReadEvent::REALTIME_MESSAGE: 
            return onRealTimeMessage(getRealTimeMessage());
        default: 
            return true;
    }
}

// -------------------------------------------------------------------------- //

// Sending MIDI

void USBMIDI_Interface::sendImpl(uint8_t header, uint8_t d1, uint8_t d2, 
                                uint8_t cn) {
    writeUSBPacket(cn, header >> 4, // CN|CIN
                    header,          // status
                    d1,              // data 1
                    d2);             // data 2
    flushUSB(); // TODO
}

void USBMIDI_Interface::sendImpl(uint8_t header, uint8_t d1, uint8_t cn) {
    sendImpl(header, d1, 0, cn);
}

void USBMIDI_Interface::sendImpl(const uint8_t *data, size_t length, 
                                 uint8_t cn) {
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
    flushUSB(); // TODO
}

void USBMIDI_Interface::sendImpl(uint8_t rt, uint8_t cn) {
    writeUSBPacket(cn, 0xF, // CN|CIN
                    rt,      // single byte
                    0,       // no data
                    0);      // no data
    flushUSB(); // TODO
}

END_CS_NAMESPACE

#endif
