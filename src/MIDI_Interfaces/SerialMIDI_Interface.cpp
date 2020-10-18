#include "SerialMIDI_Interface.hpp"
#include <MIDI_Parsers/StreamPuller.hpp>

BEGIN_CS_NAMESPACE

// -------------------------------------------------------------------------- //

// Reading MIDI

MIDIReadEvent StreamMIDI_Interface::read() {
    return parser.pull(StreamPuller(stream));
}

void StreamMIDI_Interface::update() { MIDI_Interface::updateIncoming(this); }

void StreamMIDI_Interface::handleStall() { MIDI_Interface::handleStall(this); }

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
