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

// -------------------------------- READING --------------------------------- //

void Parsing_MIDI_Interface::update() {
    if (event == MIDIReadEvent::NO_MESSAGE) // If previous event was handled
        event = read();                     // Read the next incoming message
    while (event != MIDIReadEvent::NO_MESSAGE) { // As long as there are
                                                 // incoming messages
        if (dispatchMIDIEvent(event)) // If event was handled successfully
            event = read();           // Read the next incoming message
        else                          // If pipe is locked
            break;                    // Try sending again next time
    }
    // TODO: maximum number of iterations? Timeout?
}

bool Parsing_MIDI_Interface::dispatchMIDIEvent(MIDIReadEvent event) {
    switch (event) {
        case MIDIReadEvent::NO_MESSAGE: return true;
        case MIDIReadEvent::CHANNEL_MESSAGE: return onChannelMessage();
        case MIDIReadEvent::SYSEX_MESSAGE: return onSysExMessage();
        case MIDIReadEvent::REALTIME_MESSAGE: return onRealTimeMessage();
        default: return true;
    }
}

bool Parsing_MIDI_Interface::onRealTimeMessage() {
    // Always send write to pipe, don't check if it's in exclusive mode or not
    auto message = getRealTimeMessage();
    sourceMIDItoPipe(message);
    if (callbacks)
        callbacks->onRealTimeMessage(*this);
    return true;
}

bool Parsing_MIDI_Interface::onChannelMessage() {
    auto message = getChannelMessage();
    if (!canWrite(message.cable))
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
    if (!canWrite(message.cable))
        return false;
    sourceMIDItoPipe(message);
    if (callbacks)
        callbacks->onSysExMessage(*this);
    return true;
}

END_CS_NAMESPACE