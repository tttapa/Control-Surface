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
    while (stream.available() > 0) {
        char data = stream.read();

        if (isxdigit(data)) {
            // if we receive a hexadecimal digit
            data = tolower(data);
            if (firstChar == '\0') {
                firstChar = data;
            } else if (secondChar == '\0') {
                secondChar = data;
            }
        }
        if (firstChar && secondChar) {
            // if we received two hex characters
            uint8_t midiByte =
                hexCharToNibble(firstChar) << 4 | hexCharToNibble(secondChar);
            firstChar = '\0';
            secondChar = '\0';
            MIDIReadEvent parseResult = parser.parse(midiByte);
            if (parseResult != MIDIReadEvent::NO_MESSAGE)
                return parseResult;
        } else if (!isxdigit(data) && firstChar) {
            // if we received one hex character followed by whitespace/other
            uint8_t midiByte = hexCharToNibble(firstChar);
            firstChar = '\0';
            MIDIReadEvent parseResult = parser.parse(midiByte);
            if (parseResult != MIDIReadEvent::NO_MESSAGE)
                return parseResult;
        } else {
            // Ignore any characters other than whitespace and hexadecimal
            // digits
        }
    }
    return MIDIReadEvent::NO_MESSAGE;
}

void StreamDebugMIDI_Interface::sendImpl(uint8_t header, uint8_t d1, uint8_t d2,
                                         uint8_t cn) {
    uint8_t messageType = (header >> 4) - 8;
    if (messageType >= 7)
        return;
    uint8_t c = header & 0x0F;
    stream << DebugMIDIMessageNames::MIDIStatusTypeNames[messageType]
           << F("\tChannel: ") << (c + 1) << F("\tData 1: 0x") << hex << d1
           << F("\tData 2: 0x") << d2 << dec << F("\tCable: ") << cn << endl;
    stream.flush();
}

void StreamDebugMIDI_Interface::sendImpl(uint8_t header, uint8_t d1,
                                         uint8_t cn) {
    uint8_t messageType = (header >> 4) - 8;
    if (messageType >= 7)
        return;
    uint8_t c = header & 0x0F;
    stream << DebugMIDIMessageNames::MIDIStatusTypeNames[messageType]
           << F("\tChannel: ") << (c + 1) << F("\tData 1: 0x") << hex << d1
           << dec << F("\tCable: ") << cn << endl;
    stream.flush();
}

void StreamDebugMIDI_Interface::sendImpl(const uint8_t *data, size_t length,
                                         uint8_t cn) {
    stream << F("SysEx           \t") << hex << uppercase;
    while (length-- > 0)
        stream << (*data++) << ' ';
    stream << dec << F("\tCable: ") << cn << "\r\n";
    stream.flush();
}

void StreamDebugMIDI_Interface::sendImpl(uint8_t rt, uint8_t cn) {
    stream << F("Real-Time: 0x") << hex << rt << dec << F("\tCable: ") << cn
           << endl;
    stream.flush();
}

END_CS_NAMESPACE