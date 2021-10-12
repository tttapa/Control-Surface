#include "DebugMIDI_Interface.hpp"
#include <AH/PrintStream/PrintStream.hpp>

BEGIN_CS_NAMESPACE

namespace DebugMIDIMessageNames {

#ifdef PROGMEM

const static char NoteOff[] PROGMEM = "Note Off         ";
const static char NoteOn[] PROGMEM = "Note On          ";
const static char KeyPressure[] PROGMEM = "Key Pressure     ";
const static char ControlChange[] PROGMEM = "Control Change   ";
const static char ProgramChange[] PROGMEM = "Program Change   ";
const static char ChannelPressure[] PROGMEM = "Channel Pressure ";
const static char PitchBend[] PROGMEM = "Pitch Bend       ";

const static FlashString_t MIDIStatusTypeNames[] = {
    reinterpret_cast<FlashString_t>(NoteOff),
    reinterpret_cast<FlashString_t>(NoteOn),
    reinterpret_cast<FlashString_t>(KeyPressure),
    reinterpret_cast<FlashString_t>(ControlChange),
    reinterpret_cast<FlashString_t>(ProgramChange),
    reinterpret_cast<FlashString_t>(ChannelPressure),
    reinterpret_cast<FlashString_t>(PitchBend),
};

#else

const static char *MIDIStatusTypeNames[] = {
    "Note Off         ", "Note On          ", "Key Pressure     ",
    "Control Change   ", "Program Change   ", "Channel Pressure ",
    "Pitch Bend       ",
};

#endif

} // namespace DebugMIDIMessageNames

MIDIReadEvent StreamDebugMIDI_Interface::read() {
    return parser.pull(hexstream);
}

void StreamDebugMIDI_Interface::update() {
    MIDI_Interface::updateIncoming(this);
}

void StreamDebugMIDI_Interface::handleStall() {
    MIDI_Interface::handleStall(this);
}

void StreamDebugMIDI_Base::sendChannelMessageImpl(Stream &stream,
                                                  ChannelMessage msg) {
    uint8_t messageType = (msg.header >> 4) - 8;
    if (messageType >= 7)
        return;

    DEBUG_LOCK_MUTEX
    if (prefix != nullptr)
        stream << prefix << ' ';
    if (msg.hasTwoDataBytes())
        stream << DebugMIDIMessageNames::MIDIStatusTypeNames[messageType]
               << F("Channel: ") << msg.getChannel().getOneBased()
               << F("\tData 1: 0x") << hex << msg.getData1()
               << F("\tData 2: 0x") << msg.getData2() << dec;
    else
        stream << DebugMIDIMessageNames::MIDIStatusTypeNames[messageType]
               << F("Channel: ") << msg.getChannel().getOneBased()
               << F("\tData 1: 0x") << hex << msg.getData1() << dec;
    if (msg.getMessageType() == msg.PITCH_BEND)
        stream << " (" << msg.getData14bit() << ')';
    if (msg.getCable() != CABLE_1)
        stream << F("\tCable: ") << msg.getCable().getOneBased();
    stream << "\r\n";
}

void StreamDebugMIDI_Base::sendSysExImpl(Stream &stream, SysExMessage msg) {
    DEBUG_LOCK_MUTEX
    if (prefix != nullptr)
        stream << prefix << ' ';
    stream << F("System Exclusive [") << msg.length
           << (msg.isLastChunk() ? "]\t" : "+]\t")
           << AH::HexDump(msg.data, msg.length);
    if (msg.getCable() != CABLE_1)
        stream << F("\tCable: ") << msg.getCable().getOneBased();
    stream << "\r\n";
}

void StreamDebugMIDI_Base::sendSysCommonImpl(Stream &stream,
                                             SysCommonMessage msg) {
    DEBUG_LOCK_MUTEX
    if (prefix != nullptr)
        stream << prefix << ' ';
    stream << F("System Common    ") << msg.getMessageType() << hex;
    if (msg.getNumberOfDataBytes() >= 1)
        stream << F("\tData 1: 0x") << msg.getData1();
    if (msg.getNumberOfDataBytes() >= 2)
        stream << F("\tData 2: 0x") << msg.getData2() << dec << " ("
               << msg.getData14bit() << ')';
    else
        stream << dec;
    if (msg.getCable() != CABLE_1)
        stream << F("\tCable: ") << msg.getCable().getOneBased();
    stream << "\r\n";
}

void StreamDebugMIDI_Base::sendRealTimeImpl(Stream &stream,
                                            RealTimeMessage msg) {
    DEBUG_LOCK_MUTEX
    if (prefix != nullptr)
        stream << prefix << ' ';
    stream << F("Real-Time        ") << msg.getMessageType();
    if (msg.getCable() != CABLE_1)
        stream << F("\tCable: ") << msg.getCable().getOneBased();
    stream << "\r\n";
}

void StreamDebugMIDI_Output::sendChannelMessageImpl(ChannelMessage m) {
    StreamDebugMIDI_Base::sendChannelMessageImpl(stream, m);
}
void StreamDebugMIDI_Output::sendSysCommonImpl(SysCommonMessage m) {
    StreamDebugMIDI_Base::sendSysCommonImpl(stream, m);
}
void StreamDebugMIDI_Output::sendSysExImpl(SysExMessage m) {
    StreamDebugMIDI_Base::sendSysExImpl(stream, m);
}
void StreamDebugMIDI_Output::sendRealTimeImpl(RealTimeMessage m) {
    StreamDebugMIDI_Base::sendRealTimeImpl(stream, m);
}
void StreamDebugMIDI_Output::sendNowImpl() {
    StreamDebugMIDI_Base::sendNowImpl(stream);
}

void StreamDebugMIDI_Interface::sendChannelMessageImpl(ChannelMessage m) {
    StreamDebugMIDI_Base::sendChannelMessageImpl(getStream(), m);
}
void StreamDebugMIDI_Interface::sendSysCommonImpl(SysCommonMessage m) {
    StreamDebugMIDI_Base::sendSysCommonImpl(getStream(), m);
}
void StreamDebugMIDI_Interface::sendSysExImpl(SysExMessage m) {
    StreamDebugMIDI_Base::sendSysExImpl(getStream(), m);
}
void StreamDebugMIDI_Interface::sendRealTimeImpl(RealTimeMessage m) {
    StreamDebugMIDI_Base::sendRealTimeImpl(getStream(), m);
}
void StreamDebugMIDI_Interface::sendNowImpl() {
    StreamDebugMIDI_Base::sendNowImpl(getStream());
}

END_CS_NAMESPACE