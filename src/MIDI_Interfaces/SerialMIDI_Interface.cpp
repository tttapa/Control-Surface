#include "SerialMIDI_Interface.hpp"
#include <MIDI_Parsers/StreamPuller.hpp>

BEGIN_CS_NAMESPACE

// -------------------------------------------------------------------------- //

// Reading MIDI

MIDIReadEvent StreamMIDI_Interface::read() {
    return parser.pull(StreamPuller(stream));
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

void StreamMIDI_Interface::sendChannelMessageImpl(ChannelMessage msg) {
    stream.write(msg.header);
    stream.write(msg.data1);
    if (msg.hasTwoDataBytes())
        stream.write(msg.data2);
}

void StreamMIDI_Interface::sendSysExImpl(SysExMessage msg) {
    stream.write(msg.data, msg.length);
}

void StreamMIDI_Interface::sendRealTimeImpl(RealTimeMessage msg) {
    stream.write(msg.message);
}

END_CS_NAMESPACE
