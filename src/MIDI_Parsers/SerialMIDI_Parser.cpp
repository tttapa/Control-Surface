#include "SerialMIDI_Parser.hpp"

BEGIN_CS_NAMESPACE

MIDIReadEvent SerialMIDI_Parser::parse(uint8_t midiByte) {
    // DEBUGFN(hex << NAMEDVALUE(midiByte) << dec);
#if !IGNORE_SYSEX
    // SysEx constants
    constexpr uint8_t SysExStart =
        static_cast<uint8_t>(MIDIMessageType::SYSEX_START);
    constexpr uint8_t SysExEnd =
        static_cast<uint8_t>(MIDIMessageType::SYSEX_END);

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

    // If it's a status byte (first byte of a message)
    if (isStatus(midiByte)) {
        // If it's a Real-Time message
        if (midiByte >= uint8_t(MIDIMessageType::TIMING_CLOCK)) {
            rtmsg.message = midiByte;
            return MIDIReadEvent::REALTIME_MESSAGE;
        }
        // Normal header (channel message, system exclusive, system common)
        else {
            // If a SysEx message was being received, and now we receive
            // another status byte, remember to correctly terminate the SysEx
            // message later
            bool unterminatedSysEx = midimsg.header == SysExStart;

            // Save the newly received status byte
            midimsg.header = midiByte;
            // A new message starts, so we haven't received the second byte yet
            thirdByte = false;

            if (midimsg.header == uint8_t(MIDIMessageType::TUNE_REQUEST)) {
                // Tune request (not implemented)
                // TODO: should I implement this as a Real-Time message?
                // (That might lead to problems when a SysEx message is
                // terminated by a Tune Request, see unterminatedSysEx below)
                // TODO: should I set midimsg.header in this case?
            }

#if !IGNORE_SYSEX
            if (unterminatedSysEx) {
                // If we're currently receiving a SysEx message
                addSysExByte(SysExEnd); // Try to add SysExEnd byte to buffer
                // Even if the buffer is full, end the message anyway
                endSysEx();
                return MIDIReadEvent::SYSEX_MESSAGE;
            }
#else
            (void)unterminatedSysEx;
#endif // IGNORE_SYSEX
        }
    }

    // If it's a data byte
    else {
        if (midimsg.header == 0) {
            DEBUGFN(F("Warning: No header"));
            ; // Ignore
        }
        // Third byte of three (data 2)
        else if (thirdByte) {
            midimsg.data2 = midiByte;
            thirdByte = false;
            return MIDIReadEvent::CHANNEL_MESSAGE;
        }
        // Second byte (data 1) or SysEx data
        else {
            // Channel message with two data bytes
            if (midimsg.hasTwoDataBytes()) {
                midimsg.data1 = midiByte;
                thirdByte = true; // expect a third byte next
            }
            // Channel message with one data byte
            else if (midimsg.hasValidHeader()) {
                midimsg.data1 = midiByte;
                return MIDIReadEvent::CHANNEL_MESSAGE;
            }
            // Not a channel message
#if !IGNORE_SYSEX
            // SysEx data byte
            else if (midimsg.header == SysExStart) {
                addSysExByte(midiByte);
            }
#endif // IGNORE_SYSEX
            else {
                DEBUGFN(F("Data byte ignored"));
            }
        }
    }
    return MIDIReadEvent::NO_MESSAGE;
}

END_CS_NAMESPACE