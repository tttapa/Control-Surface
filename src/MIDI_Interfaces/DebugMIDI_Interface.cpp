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

void StreamDebugMIDI_Interface::sendImpl(uint8_t header, uint8_t d1, uint8_t d2,
                                         Cable cn) {
    uint8_t messageType = (header >> 4) - 8;
    if (messageType >= 7)
        return;
    uint8_t c = header & 0x0F;
    getStream() << DebugMIDIMessageNames::MIDIStatusTypeNames[messageType]
                << F("\tChannel: ") << (c + 1) << F("\tData 1: 0x") << hex << d1
                << F("\tData 2: 0x") << d2 << dec << F("\tCable: ")
                << cn.getOneBased() << endl;
}

void StreamDebugMIDI_Interface::sendImpl(uint8_t header, uint8_t d1, Cable cn) {
    uint8_t messageType = (header >> 4) - 8;
    if (messageType >= 7)
        return;
    uint8_t c = header & 0x0F;
    getStream() << DebugMIDIMessageNames::MIDIStatusTypeNames[messageType]
                << F("\tChannel: ") << (c + 1) << F("\tData 1: 0x") << hex << d1
                << dec << F("\tCable: ") << cn.getOneBased() << endl;
}

void StreamDebugMIDI_Interface::sendImpl(const uint8_t *data, size_t length,
                                         Cable cn) {
    getStream() << F("SysEx           \t") << AH::HexDump(data, length)
                << F("\tCable: ") << cn.getOneBased() << "\r\n";
}

void StreamDebugMIDI_Interface::sendImpl(uint8_t rt, Cable cn) {
    getStream() << F("Real-Time: 0x") << hex << uppercase << rt << dec
                << F("\tCable: ") << cn.getOneBased() << endl;
}

END_CS_NAMESPACE