#include "DebugMIDI_Interface.hpp"
#include <AH/PrintStream/PrintStream.hpp>

BEGIN_CS_NAMESPACE

namespace DebugMIDIMessageNames {

#ifdef PROGMEM

const static char NoteOff[] PROGMEM = "Note Off        ";
const static char NoteOn[] PROGMEM = "Note On         ";
const static char KeyPressure[] PROGMEM = "Key Pressure    ";
const static char ControlChange[] PROGMEM = "Control Change  ";
const static char ProgramChange[] PROGMEM = "Program Change  ";
const static char ChannelPressure[] PROGMEM = "Channel Pressure";
const static char PitchBend[] PROGMEM = "Pitch Bend      ";

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
    "Note Off\t",       "Note On\t\t",      "Key Pressure\t",
    "Control Change\t", "Program Change\t", "Channel Pressure",
    "Pitch Bend\t",
};

#endif

} // namespace DebugMIDIMessageNames

MIDIReadEvent StreamDebugMIDI_Interface::read() {
    return parser.pull(hexstream);
}

void StreamDebugMIDI_Interface::update() {
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

bool StreamDebugMIDI_Interface::dispatchMIDIEvent(MIDIReadEvent event) {
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

void StreamDebugMIDI_Interface::sendChannelMessageImpl(ChannelMessage msg) {
    uint8_t messageType = (msg.header >> 4) - 8;
    if (messageType >= 7)
        return;

    if (msg.hasTwoDataBytes())
        getStream() << DebugMIDIMessageNames::MIDIStatusTypeNames[messageType]
                    << F("\tChannel: ") << msg.getChannel().getOneBased()
                    << F("\tData 1: 0x") << hex << msg.getData1()
                    << F("\tData 2: 0x") << msg.getData2() << dec
                    << F("\tCable: ") << msg.getCable().getOneBased() << endl;
    else
        getStream() << DebugMIDIMessageNames::MIDIStatusTypeNames[messageType]
                    << F("\tChannel: ") << msg.getChannel().getOneBased()
                    << F("\tData 1: 0x") << hex << msg.getData1() << dec
                    << F("\tCable: ") << msg.getCable().getOneBased() << endl;
}

void StreamDebugMIDI_Interface::sendSysExImpl(SysExMessage msg) {
    getStream() << F("SysEx           \t") << AH::HexDump(msg.data, msg.length)
                << F("\tCable: ") << msg.getCable().getOneBased() << "\r\n";
}

void StreamDebugMIDI_Interface::sendRealTimeImpl(RealTimeMessage msg) {
    getStream() << F("Real-Time: 0x") << hex << uppercase << msg.message << dec
                << F("\tCable: ") << msg.getCable().getOneBased() << endl;
}

END_CS_NAMESPACE