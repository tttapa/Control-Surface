#include "SerialMIDI_Interface.hpp"

BEGIN_CS_NAMESPACE

// -------------------------------------------------------------------------- //

// Reading MIDI

MIDIReadEvent StreamMIDI_Interface::read() {
    // TODO: prevent hanging up on very long SysEx messages
    while (stream.available() > 0) {
        uint8_t midiByte = stream.read();
        MIDIReadEvent parseResult = parser.parse(midiByte);
        if (parseResult != MIDIReadEvent::NO_MESSAGE)
            return parseResult;
    }
    return MIDIReadEvent::NO_MESSAGE;
}

void StreamMIDI_Interface::update() {
    MIDIReadEvent event = read();
    while (event != MIDIReadEvent::NO_MESSAGE) {
        dispatchMIDIEvent(event);
        event = read();
    }
    // TODO: add logic to detect MIDI messages such as (N)RPN that span over 
    // multiple channel messages and that shouldn't be interrupted.
    // For short messages such as (N)RPN, I suggest waiting with a timeout, 
    // for chunked SysEx, it's probably best to stall the pipe and return.
}

bool StreamMIDI_Interface::dispatchMIDIEvent(MIDIReadEvent event) {
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

void StreamMIDI_Interface::sendImpl(uint8_t header, uint8_t d1, uint8_t d2,
                                    uint8_t cn) {
    (void)cn;
    stream.write(header); // Send the MIDI message over the stream
    stream.write(d1);
    stream.write(d2);
}

void StreamMIDI_Interface::sendImpl(uint8_t header, uint8_t d1, uint8_t cn) {
    (void)cn;
    stream.write(header); // Send the MIDI message over the stream
    stream.write(d1);
}

void StreamMIDI_Interface::sendImpl(const uint8_t *data, size_t length,
                                    uint8_t cn) {
    (void)cn;
    stream.write(data, length);
}

void StreamMIDI_Interface::sendImpl(uint8_t rt, uint8_t cn) {
    (void)cn;
    stream.write(rt); // Send the MIDI message over the stream
}

END_CS_NAMESPACE
