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

// Retrieving the received messages

ChannelMessage StreamMIDI_Interface::getChannelMessage() const {
    return parser.getChannelMessage();
}

SysCommonMessage StreamMIDI_Interface::getSysCommonMessage() const {
    return parser.getSysCommonMessage();
}

RealTimeMessage StreamMIDI_Interface::getRealTimeMessage() const {
    return parser.getRealTimeMessage();
}

SysExMessage StreamMIDI_Interface::getSysExMessage() const {
    return parser.getSysExMessage();
}

// -------------------------------------------------------------------------- //

// Sending MIDI

void StreamMIDI_Interface::sendChannelMessageImpl(ChannelMessage msg) {
    stream.write(msg.header);
    stream.write(msg.data1);
    if (msg.hasTwoDataBytes())
        stream.write(msg.data2);
}

void StreamMIDI_Interface::sendSysCommonImpl(SysCommonMessage msg) {
    stream.write(msg.header);
    if (msg.getNumberOfDataBytes() >= 1)
        stream.write(msg.data1);
    if (msg.getNumberOfDataBytes() >= 2)
        stream.write(msg.data2);
}

void StreamMIDI_Interface::sendSysExImpl(SysExMessage msg) {
    stream.write(msg.data, msg.length);
}

void StreamMIDI_Interface::sendRealTimeImpl(RealTimeMessage msg) {
    stream.write(msg.message);
}

END_CS_NAMESPACE
