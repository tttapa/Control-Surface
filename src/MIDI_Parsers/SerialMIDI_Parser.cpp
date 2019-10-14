#include "SerialMIDI_Parser.hpp"

BEGIN_CS_NAMESPACE

MIDI_read_t SerialMIDI_Parser::parse(uint8_t midiByte) {
    DEBUGFN(hex << NAMEDVALUE(midiByte) << dec);
#if !IGNORE_SYSEX
    // If the previous byte was a SysExStart
    // I have to handle a start in the next time step, because a start can also
    // end the previous message. When that happens, I have to return 
    // SYSEX_MESSAGE without resetting the buffer.
    // Then, after handling the message by the user, I do have to reset the 
    // buffer.
    if (midimsg.header == SysExStart && !sysexbuffer.isReceiving()) {
        startSysEx();
        addSysExByte(SysExStart);
    }
#endif
    if (isStatus(midiByte)) {
        // If it's a status byte (first byte)
        if (midiByte >= 0xF8) {
            // If it's a Real-Time message
            return static_cast<MIDI_read_t>(midiByte);
        } else {
            // Normal header
            uint8_t previousHeader = midimsg.header;
            midimsg.header = midiByte;
            thirdByte = false;
#if !IGNORE_SYSEX
            if (midimsg.header == TuneRequest) {
                ; // Tune request (not implemented)
            }
            if (previousHeader == SysExStart) {
                // If we're currently receiving a SysEx message
                addSysExByte(SysExEnd); // Try to add SysExEnd byte to buffer
                // Even if the buffer is full, end the message anyway
                endSysEx();
                return SYSEX_MESSAGE;
            }
#else
            (void)previousHeader;
#endif // IGNORE_SYSEX
        }
    } else {
        // If it's a data byte
        if (midimsg.header == 0) {
            DEBUGFN("Warning: No header");
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
#if !IGNORE_SYSEX
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

END_CS_NAMESPACE