#include "MIDI_Interface.hpp"

BEGIN_CS_NAMESPACE

MIDI_Interface::MIDI_Interface() {
    setAsDefault(); // Make this the default MIDI Interface
}

MIDI_Interface::~MIDI_Interface() {
    if (getDefault() == this)
        DefaultMIDI_Interface = nullptr;
}

MIDI_Interface *MIDI_Interface::DefaultMIDI_Interface = nullptr;

void MIDI_Interface::setAsDefault() { DefaultMIDI_Interface = this; }

MIDI_Interface *MIDI_Interface::getDefault() { return DefaultMIDI_Interface; }

// -------------------------------- SENDING --------------------------------- //

void MIDI_Interface::send(uint8_t m, uint8_t c, uint8_t d1, uint8_t d2) {
    sendOnCable(m, c, d1, d2, 0);
}

void MIDI_Interface::send(uint8_t m, uint8_t c, uint8_t d1) {
    sendOnCable(m, c, d1, 0);
}

void MIDI_Interface::sendOnCable(uint8_t m, uint8_t c, uint8_t d1, uint8_t d2,
                                 uint8_t cn) {
    c--;             // Channels are zero-based
    m &= 0xF0;       // bitmask high nibble
    m |= 0b10000000; // set msb
    c &= 0x0F;       // bitmask low nibble
    d1 &= 0x7F;      // clear msb
    d2 &= 0x7F;      // clear msb
    cn &= 0x0F;      // bitmask low nibble
    sendImpl(m, c, d1, d2, cn);
}

void MIDI_Interface::sendOnCable(uint8_t m, uint8_t c, uint8_t d1, uint8_t cn) {
    c--;             // Channels are zero-based
    m &= 0xF0;       // bitmask high nibble
    m |= 0b10000000; // set msb
    c &= 0x0F;       // bitmask low nibble
    d1 &= 0x7F;      // clear msb
    cn &= 0x0F;      // bitmask low nibble
    sendImpl(m, c, d1, cn);
}

void MIDI_Interface::sendOnCable(uint8_t r, uint8_t cn) {
    r |= 0b10000000; // set msb
    cn &= 0x0F;      // bitmask low nibble
    sendImpl(r, cn);
}

void MIDI_Interface::sendNoteOn(MIDICNChannelAddress address,
                                uint8_t velocity) {
    if (address)
        sendImpl(NOTE_ON, address.getRawChannel(), address.getAddress(),
                 velocity, address.getCableNumber());
}
void MIDI_Interface::sendNoteOff(MIDICNChannelAddress address,
                                 uint8_t velocity) {
    if (address)
        sendImpl(NOTE_OFF, address.getRawChannel(), address.getAddress(),
                 velocity, address.getCableNumber());
}
void MIDI_Interface::sendKP(MIDICNChannelAddress address, uint8_t pressure) {
    if (address)
        sendImpl(KEY_PRESSURE, address.getRawChannel(), address.getAddress(),
                 pressure, address.getCableNumber());
}
void MIDI_Interface::sendCC(MIDICNChannelAddress address, uint8_t value) {
    if (address)
        sendImpl(CC, address.getRawChannel(), address.getAddress(), value,
                 address.getCableNumber());
}
void MIDI_Interface::sendPC(MIDICNChannel address, uint8_t value) {
    if (address)
        sendImpl(PROGRAM_CHANGE, address.getRawChannel(), value,
                 address.getCableNumber());
}
void MIDI_Interface::sendPC(MIDICNChannelAddress address) {
    if (address)
        sendImpl(PROGRAM_CHANGE, address.getRawChannel(), address.getAddress(),
                 address.getCableNumber());
}
void MIDI_Interface::sendCP(MIDICNChannel address, uint8_t pressure) {
    if (address)
        sendImpl(CHANNEL_PRESSURE, address.getRawChannel(), pressure,
                 address.getCableNumber());
}
void MIDI_Interface::sendPB(MIDICNChannel address, uint16_t value) {
    if (address)
        sendImpl(PITCH_BEND, address.getRawChannel(), value & 0x7F, value >> 7,
                 address.getCableNumber());
}
void MIDI_Interface::send(SysExMessage message) {
    if (message.length) {
        if (message.length < 2) {
            ERROR(F("Error: invalid SysEx length"), 0x7F7F);
            return;
        }
        sendImpl(message.data, message.length, message.CN);
    }
}
void MIDI_Interface::send(uint8_t rt, uint8_t cn) {
    if (rt) {
        sendImpl(rt, cn);
    }
}

// -------------------------------- PARSING --------------------------------- //

Parsing_MIDI_Interface::Parsing_MIDI_Interface(MIDI_Parser &parser)
    : parser(parser) {}

ChannelMessage Parsing_MIDI_Interface::getChannelMessage() {
    return parser.getChannelMessage();
}

SysExMessage Parsing_MIDI_Interface::getSysExMessage() const {
    return parser.getSysEx();
}

uint8_t Parsing_MIDI_Interface::getCN() const { return parser.getCN(); }

// -------------------------------- READING --------------------------------- //

void Parsing_MIDI_Interface::update() {
    bool repeat = true;
    while (repeat) {
        MIDI_read_t event = read();
        repeat = dispatchMIDIEvent(event);
    }
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wswitch-enum"

bool Parsing_MIDI_Interface::dispatchMIDIEvent(MIDI_read_t event) {
    switch (event) {
        case NO_MESSAGE: return false;
        case CHANNEL_MESSAGE: onChannelMessage(); return true;
        case SYSEX_MESSAGE: onSysExMessage(); return true;
        default: onRealtimeMessage(static_cast<uint8_t>(event)); return true;
    }
}

#pragma GCC diagnostic pop

void Parsing_MIDI_Interface::onRealtimeMessage(uint8_t message) {
    if (callbacks)
        callbacks->onRealtimeMessage(*this, message);
}

void Parsing_MIDI_Interface::onChannelMessage() {
    if (callbacks)
        callbacks->onChannelMessage(*this);
}

void Parsing_MIDI_Interface::onSysExMessage() {
    if (callbacks)
        callbacks->onSysExMessage(*this);
}

END_CS_NAMESPACE