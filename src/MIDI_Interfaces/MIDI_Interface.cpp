#include "MIDI_Interface.hpp"

BEGIN_CS_NAMESPACE

MIDI_Interface::~MIDI_Interface() {
    if (getDefault() == this)
        DefaultMIDI_Interface = nullptr;
}

MIDI_Interface *MIDI_Interface::DefaultMIDI_Interface = nullptr;

void MIDI_Interface::setAsDefault() { DefaultMIDI_Interface = this; }

MIDI_Interface *MIDI_Interface::getDefault() {
#ifndef ARDUINO
    return DefaultMIDI_Interface == nullptr
               ? dynamic_cast<MIDI_Interface *>(updatables.getLast())
               : DefaultMIDI_Interface;
#else
    return DefaultMIDI_Interface == nullptr
               ? static_cast<MIDI_Interface *>(updatables.getLast())
               : DefaultMIDI_Interface;
#endif
}

// -------------------------------- SENDING --------------------------------- //

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