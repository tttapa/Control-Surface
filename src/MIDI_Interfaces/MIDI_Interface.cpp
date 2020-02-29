#include "MIDI_Interface.hpp"

BEGIN_CS_NAMESPACE

MIDI_Interface::MIDI_Interface() {
    setAsDefault(); // Make this the default MIDI Interface
}

MIDI_Interface::MIDI_Interface(MIDI_Interface &&) {
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

void MIDI_Interface::sendNoteOn(MIDIAddress address, uint8_t velocity) {
    if (address)
        sendImpl(NOTE_ON, address.getRawChannel(), address.getAddress(),
                 velocity, address.getCableNumber());
}
void MIDI_Interface::sendNoteOff(MIDIAddress address, uint8_t velocity) {
    if (address)
        sendImpl(NOTE_OFF, address.getRawChannel(), address.getAddress(),
                 velocity, address.getCableNumber());
}
void MIDI_Interface::sendKP(MIDIAddress address, uint8_t pressure) {
    if (address)
        sendImpl(KEY_PRESSURE, address.getRawChannel(), address.getAddress(),
                 pressure, address.getCableNumber());
}
void MIDI_Interface::sendCC(MIDIAddress address, uint8_t value) {
    if (address)
        sendImpl(CC, address.getRawChannel(), address.getAddress(), value,
                 address.getCableNumber());
}
void MIDI_Interface::sendPC(MIDIChannelCN address, uint8_t value) {
    if (address)
        sendImpl(PROGRAM_CHANGE, address.getRawChannel(), value,
                 address.getCableNumber());
}
void MIDI_Interface::sendPC(MIDIAddress address) {
    if (address)
        sendImpl(PROGRAM_CHANGE, address.getRawChannel(), address.getAddress(),
                 address.getCableNumber());
}
void MIDI_Interface::sendCP(MIDIChannelCN address, uint8_t pressure) {
    if (address)
        sendImpl(CHANNEL_PRESSURE, address.getRawChannel(), pressure,
                 address.getCableNumber());
}
void MIDI_Interface::sendPB(MIDIChannelCN address, uint16_t value) {
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

void MIDI_Interface::send(RealTimeMessage message) {
    send(message.message, message.CN);
}

void MIDI_Interface::send(ChannelMessage message) {
    uint8_t m = message.header & 0xF0; // message type
    uint8_t c = message.header & 0x0F; // channel
    // TODO: optimize header?
    if (m != PROGRAM_CHANGE && m != CHANNEL_PRESSURE)
        sendImpl(m, c, message.data1, message.data2, message.CN);
    else
        sendImpl(m, c, message.data1, message.CN);
}

void MIDI_Interface::sinkMIDIfromPipe(ChannelMessage msg) { send(msg); }
void MIDI_Interface::sinkMIDIfromPipe(SysExMessage msg) { send(msg); }
void MIDI_Interface::sinkMIDIfromPipe(RealTimeMessage msg) { send(msg); }

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
    if (event == NO_MESSAGE)          // If previous event was handled
        event = read();               // Read the next incoming message
    while (event != NO_MESSAGE) {     // As long as there are incoming messages
        if (dispatchMIDIEvent(event)) // If handled successfully
            event = read();           // Read the next incoming message
        else                          // If pipe is locked
            break;                    // Try sending again next time
    }
    // TODO: maximum number of iterations? Timeout?
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wswitch-enum"

bool Parsing_MIDI_Interface::dispatchMIDIEvent(MIDI_read_t event) {
    switch (event) {
        case NO_MESSAGE: return true;
        case CHANNEL_MESSAGE: return onChannelMessage();
        case SYSEX_MESSAGE: return onSysExMessage();
        default: return onRealtimeMessage(static_cast<uint8_t>(event));
    }
}

#pragma GCC diagnostic pop

bool Parsing_MIDI_Interface::onRealtimeMessage(uint8_t message) {
    // Always send write to pipe, don't check if it's in exclusive mode or not
    sourceMIDItoPipe(RealTimeMessage{message, getCN()});
    if (callbacks)
        callbacks->onRealtimeMessage(*this, message);
    return true;
}

bool Parsing_MIDI_Interface::onChannelMessage() {
    auto message = getChannelMessage();
    if (!canWrite(message.CN))
        return false;
    sourceMIDItoPipe(message);
    if (callbacks)
        callbacks->onChannelMessage(*this);
    // TODO: we have the message already, should we just pass it to the
    //       callback as a parameter? (idem for SysEx and RT)
    return true;
}

bool Parsing_MIDI_Interface::onSysExMessage() {
    auto message = getSysExMessage();
    if (!canWrite(message.CN))
        return false;
    sourceMIDItoPipe(message);
    if (callbacks)
        callbacks->onSysExMessage(*this);
    return true;
}

END_CS_NAMESPACE