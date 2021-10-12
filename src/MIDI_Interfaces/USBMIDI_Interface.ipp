#include <MIDI_Parsers/LambdaPuller.hpp>

BEGIN_CS_NAMESPACE

// Reading MIDI
// -----------------------------------------------------------------------------

template <class Backend>
MIDIReadEvent GenericUSBMIDI_Interface<Backend>::read() {
    auto pullpacket = [this](typename Backend::MIDIUSBPacket_t &packet) {
        packet = backend.read();
        return packet[0] != 0x00;
    };
    return parser.pull(LambdaPuller(std::move(pullpacket)));
}

template <class Backend>
void GenericUSBMIDI_Interface<Backend>::update() {
    MIDI_Interface::updateIncoming(this);
}

template <class Backend>
void GenericUSBMIDI_Interface<Backend>::handleStall() {
    MIDI_Interface::handleStall(this);
}

// Retrieving the received messages
// -----------------------------------------------------------------------------

template <class Backend>
ChannelMessage GenericUSBMIDI_Interface<Backend>::getChannelMessage() const {
    return parser.getChannelMessage();
}

template <class Backend>
SysCommonMessage
GenericUSBMIDI_Interface<Backend>::getSysCommonMessage() const {
    return parser.getSysCommonMessage();
}

template <class Backend>
RealTimeMessage GenericUSBMIDI_Interface<Backend>::getRealTimeMessage() const {
    return parser.getRealTimeMessage();
}

template <class Backend>
SysExMessage GenericUSBMIDI_Interface<Backend>::getSysExMessage() const {
    return parser.getSysExMessage();
}

// Sending MIDI
// -----------------------------------------------------------------------------

template <class Backend>
void GenericUSBMIDI_Interface<Backend>::sendChannelMessageImpl(
    ChannelMessage msg) {
    sender.sendChannelMessage(msg, Sender{this});
    if (alwaysSendImmediately_)
        backend.sendNow();
}

template <class Backend>
void GenericUSBMIDI_Interface<Backend>::sendSysCommonImpl(
    SysCommonMessage msg) {
    sender.sendSysCommonMessage(msg, Sender{this});
    if (alwaysSendImmediately_)
        backend.sendNow();
}

template <class Backend>
void GenericUSBMIDI_Interface<Backend>::sendSysExImpl(const SysExMessage msg) {
    sender.sendSysEx(msg, Sender{this});
    if (alwaysSendImmediately_)
        backend.sendNow();
}

template <class Backend>
void GenericUSBMIDI_Interface<Backend>::sendRealTimeImpl(RealTimeMessage msg) {
    sender.sendRealTimeMessage(msg, Sender{this});
    if (alwaysSendImmediately_)
        backend.sendNow();
}

END_CS_NAMESPACE
