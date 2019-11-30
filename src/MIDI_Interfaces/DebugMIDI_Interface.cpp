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

const static __FlashStringHelper *MIDIStatusTypeNames[] = {
    reinterpret_cast<const __FlashStringHelper *>(NoteOff),
    reinterpret_cast<const __FlashStringHelper *>(NoteOn),
    reinterpret_cast<const __FlashStringHelper *>(KeyPressure),
    reinterpret_cast<const __FlashStringHelper *>(ControlChange),
    reinterpret_cast<const __FlashStringHelper *>(ProgramChange),
    reinterpret_cast<const __FlashStringHelper *>(ChannelPressure),
    reinterpret_cast<const __FlashStringHelper *>(PitchBend),
};

#else

const static char *MIDIStatusTypeNames[] = {
    "Note Off\t",       "Note On\t\t",      "Key Pressure\t",
    "Control Change\t", "Program Change\t", "Channel Pressure",
    "Pitch Bend\t",
};

#endif

} // namespace DebugMIDIMessageNames

MIDI_read_t StreamDebugMIDI_Interface::read() {
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
            MIDI_read_t parseResult = parser.parse(midiByte);
            if (parseResult != NO_MESSAGE)
                return parseResult;
        } else if (!isxdigit(data) && firstChar) {
            // if we received one hex character followed by whitespace/other
            uint8_t midiByte = hexCharToNibble(firstChar);
            firstChar = '\0';
            MIDI_read_t parseResult = parser.parse(midiByte);
            if (parseResult != NO_MESSAGE)
                return parseResult;
        } else {
            // Ignore any characters other than whitespace and hexadecimal
            // digits
        }
    }
    return NO_MESSAGE;
}

void StreamDebugMIDI_Interface::sendImpl(uint8_t m, uint8_t c, uint8_t d1,
                                         uint8_t d2, uint8_t cn) {
    uint8_t messageType = (m >> 4) - 8;
    if (messageType >= 7)
        return;
    stream << DebugMIDIMessageNames::MIDIStatusTypeNames[messageType]
           << F("\tChannel: ") << (c + 1) << F("\tData 1: 0x") << hex << d1
           << F("\tData 2: 0x") << d2 << dec << F("\tCable: ") << cn << endl;
    stream.flush();
}

void StreamDebugMIDI_Interface::sendImpl(uint8_t m, uint8_t c, uint8_t d1,
                                         uint8_t cn) {
    uint8_t messageType = (m >> 4) - 8;
    if (messageType >= 7)
        return;
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