#include "DebugMIDI_Interface.hpp"
#include <AH/PrintStream/PrintStream.hpp>

#include "PicoUSBInit.hpp"

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
    if (!ensure_usb_init(getStream()))
        return MIDIReadEvent::NO_MESSAGE;
    return parser.pull(hexstream);
}

void StreamDebugMIDI_Interface::update() {
    MIDI_Interface::updateIncoming(this);
}

void PrintDebugMIDI_Base::sendChannelMessageImpl(Print &stream,
                                                 ChannelMessage msg) {
    if (!ensure_usb_init(stream))
        return;
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
    if (msg.getMessageType() == msg.PitchBend)
        stream << " (" << msg.getData14bit() << ')';
    if (msg.getCable() != Cable_1)
        stream << F("\tCable: ") << msg.getCable().getOneBased();
    stream << "\r\n";
}

void PrintDebugMIDI_Base::sendSysExImpl(Print &stream, SysExMessage msg) {
    if (!ensure_usb_init(stream))
        return;
    DEBUG_LOCK_MUTEX
    if (prefix != nullptr)
        stream << prefix << ' ';
    stream << F("System Exclusive [") << msg.length
           << (msg.isLastChunk() ? "]\t" : "+]\t")
           << AH::HexDump(msg.data, msg.length);
    if (msg.getCable() != Cable_1)
        stream << F("\tCable: ") << msg.getCable().getOneBased();
    stream << "\r\n";
}

void PrintDebugMIDI_Base::sendSysCommonImpl(Print &stream,
                                            SysCommonMessage msg) {
    if (!ensure_usb_init(stream))
        return;
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
    if (msg.getCable() != Cable_1)
        stream << F("\tCable: ") << msg.getCable().getOneBased();
    stream << "\r\n";
}

void PrintDebugMIDI_Base::sendRealTimeImpl(Print &stream, RealTimeMessage msg) {
    if (!ensure_usb_init(stream))
        return;
    DEBUG_LOCK_MUTEX
    if (prefix != nullptr)
        stream << prefix << ' ';
    stream << F("Real-Time        ") << msg.getMessageType();
    if (msg.getCable() != Cable_1)
        stream << F("\tCable: ") << msg.getCable().getOneBased();
    stream << "\r\n";
}

void StreamDebugMIDI_Output::sendChannelMessageImpl(ChannelMessage m) {
    PrintDebugMIDI_Base::sendChannelMessageImpl(getStream(), m);
}
void StreamDebugMIDI_Output::sendSysCommonImpl(SysCommonMessage m) {
    PrintDebugMIDI_Base::sendSysCommonImpl(getStream(), m);
}
void StreamDebugMIDI_Output::sendSysExImpl(SysExMessage m) {
    PrintDebugMIDI_Base::sendSysExImpl(getStream(), m);
}
void StreamDebugMIDI_Output::sendRealTimeImpl(RealTimeMessage m) {
    PrintDebugMIDI_Base::sendRealTimeImpl(getStream(), m);
}
void StreamDebugMIDI_Output::sendNowImpl() {
    PrintDebugMIDI_Base::sendNowImpl(getStream());
}

void StreamDebugMIDI_Interface::sendChannelMessageImpl(ChannelMessage m) {
    PrintDebugMIDI_Base::sendChannelMessageImpl(getStream(), m);
}
void StreamDebugMIDI_Interface::sendSysCommonImpl(SysCommonMessage m) {
    PrintDebugMIDI_Base::sendSysCommonImpl(getStream(), m);
}
void StreamDebugMIDI_Interface::sendSysExImpl(SysExMessage m) {
    PrintDebugMIDI_Base::sendSysExImpl(getStream(), m);
}
void StreamDebugMIDI_Interface::sendRealTimeImpl(RealTimeMessage m) {
    PrintDebugMIDI_Base::sendRealTimeImpl(getStream(), m);
}
void StreamDebugMIDI_Interface::sendNowImpl() {
    PrintDebugMIDI_Base::sendNowImpl(getStream());
}

END_CS_NAMESPACE