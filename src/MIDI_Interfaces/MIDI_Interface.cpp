#include "MIDI_Interface.h"

MIDI_Interface::MIDI_Interface(MIDI_Parser &parser) : parser(parser) {
    setAsDefault(); // Make this the default MIDI Interface
}

MIDI_Interface::~MIDI_Interface() {
    if (getDefault() == this)
        DefaultMIDI_Interface = nullptr;
}

MIDI_Interface *MIDI_Interface::DefaultMIDI_Interface = nullptr;

void MIDI_Interface::setAsDefault() { DefaultMIDI_Interface = this; }

MIDI_Interface *MIDI_Interface::getDefault() { return DefaultMIDI_Interface; }

// -------------------------------- READING --------------------------------- //

void MIDI_Interface::update() {
    if (callbacks == nullptr)
        return;
    bool repeat = true;
    while (repeat) {
        MIDI_read_t event = read();
        repeat = dispatchMIDIEvent(event);
    }
}

bool MIDI_Interface::dispatchMIDIEvent(MIDI_read_t event) {
    switch (event) {
        case NO_MESSAGE: return false;
        case CHANNEL_MESSAGE: onChannelMessage(); return true;
        case SYSEX_MESSAGE: onSysExMessage(); return true;
        default: onRealtimeMessage(static_cast<uint8_t>(event)); return true;
    }
}

void MIDI_Interface::onRealtimeMessage(uint8_t message) {
    if (callbacks)
        callbacks->onRealtimeMessage(*this, message);
}

void MIDI_Interface::onChannelMessage() {
    if (callbacks)
        callbacks->onChannelMessage(*this);
}

void MIDI_Interface::onSysExMessage() {
    if (callbacks)
        callbacks->onSysExMessage(*this);
}

// -------------------------------- SENDING --------------------------------- //

void MIDI_Interface::send(uint8_t m, uint8_t c, uint8_t d1, uint8_t d2) {
    c--;             // Channels are zero-based
    m &= 0xF0;       // bitmask high nibble
    m |= 0b10000000; // set msb
    c &= 0xF;        // bitmask low nibble
    d1 &= 0x7F;      // clear msb
    d2 &= 0x7F;      // clear msb

    sendImpl(m, c, d1, d2);
}

void MIDI_Interface::send(uint8_t m, uint8_t c, uint8_t d1) {
    c--;             // Channels are zero-based
    m &= 0xF0;       // bitmask high nibble
    m |= 0b10000000; // set msb
    c &= 0xF;        // bitmask low nibble
    d1 &= 0x7F;      // clear msb

    sendImpl(m, c, d1);
}

void MIDI_Interface::sendNoteOn(MIDICNChannelAddress address,
                                uint8_t velocity) {
    sendImpl(NOTE_ON, address.getChannel(), address.getAddress(), velocity);
}
void MIDI_Interface::sendNoteOff(MIDICNChannelAddress address,
                                 uint8_t velocity) {
    sendImpl(NOTE_OFF, address.getChannel(), address.getAddress(), velocity);
}
void MIDI_Interface::sendCC(MIDICNChannelAddress address, uint8_t value) {
    sendImpl(CC, address.getChannel(), address.getAddress(), value);
}
void MIDI_Interface::sendPB(MIDICNChannelAddress address, uint16_t value) {
    sendImpl(PITCH_BEND, address.getChannel(), value & 0x7F, value >> 7);
}
void MIDI_Interface::sendPB(Channel channel, uint16_t value) {
    sendImpl(PITCH_BEND, channel, value & 0x7F, value >> 7);
}

// -------------------------------- PARSING --------------------------------- //

MIDI_message MIDI_Interface::getChannelMessage() {
    return parser.getChannelMessage();
}

const uint8_t *MIDI_Interface::getSysExBuffer() {
    return parser.getSysExBuffer();
}

size_t MIDI_Interface::getSysExLength() { return parser.getSysExLength(); }