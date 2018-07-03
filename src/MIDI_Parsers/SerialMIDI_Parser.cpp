#include "SerialMIDI_Parser.h"

MIDI_read_t SerialMIDI_Parser::parse(uint8_t midiByte) {
    if (isHeader(midiByte)) {
        // If it's a header byte (first byte)
        if ((midiByte & 0xF8) == 0xF8) {
            // If it's a Real-Time message (not implemented)
            ; // Handle Real-Time stuff (not implemented)
        } else {
            // Normal header
#ifndef IGNORE_SYSEX
            if (midimsg.header == SysExStart) {
                // If we're currently receiving a SysEx message
                midimsg.header = SysExEnd;
                if (addSysExByte(SysExEnd)) // add SysExEnd byte to buffer
                    return SYSEX_MESSAGE;
            }
#endif
            midimsg.header = midiByte;
            thirdByte = false;

#ifndef IGNORE_SYSEX
            if (midimsg.header == SysExStart) {
                // If the first byte of a SysEx message
                startSysEx();
                addSysExByte(SysExStart);
            }
#endif // IGNORE_SYSEX
        }
    } else {
        // If it's a data byte
        if (midimsg.header == 0) {
            DEBUGFN("Error: No header");
            ; // Ignore
        } else if (thirdByte) {
            // Third byte of three (data 2)
            midimsg.data2 = midiByte;
            thirdByte = false;
            return CHANNEL_MESSAGE;
        } else {
            // Second byte (data 1) or SysEx data
            if (midimsg.header < 0xC0 || midimsg.header == 0xE0) {
                // Note, Aftertouch, CC or Pitch Bend
                midimsg.data1 = midiByte;
                thirdByte = true;
            } else if (midimsg.header < 0xE0) {
                // Program Change or Channel Pressure
                midimsg.data1 = midiByte;
                return CHANNEL_MESSAGE;
            }
#ifndef IGNORE_SYSEX
            else if (midimsg.header == SysExStart) {
                // SysEx data byte
                addSysExByte(midiByte);
            }
#endif // IGNORE_SYSEX
            else {
                DEBUGFN("Data byte ignored");
            }
        }
    }
    return NO_MESSAGE;
}