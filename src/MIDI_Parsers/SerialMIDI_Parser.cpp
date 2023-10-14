#include "SerialMIDI_Parser.hpp"

BEGIN_CS_NAMESPACE

MIDIReadEvent SerialMIDI_Parser::handleRealTime(uint8_t midiByte) {
    rtmsg.message = midiByte;
    return MIDIReadEvent::REALTIME_MESSAGE;
}

MIDIReadEvent SerialMIDI_Parser::handleNonRealTimeStatus(uint8_t midiByte) {
#if !IGNORE_SYSEX
    // If a SysEx message was being received, and now we receive another
    // status byte, the status byte should terminate the SysEx message
    // first, and then we can handle the new status byte later.
    bool untermSysEx = currentHeader == uint8_t(MIDIMessageType::SysExStart);
    if (untermSysEx) {
        // Handle this new status byte later (unless it's just a SysEx End
        // byte, in which case we can just terminate it now).
        if (midiByte != uint8_t(MIDIMessageType::SysExEnd))
            storeByte(midiByte);
        // Terminate the SysEx message.
        // Check if the SysEx buffer has enough space to store the end byte.
        if (!hasSysExSpace()) {
            // If not store the new status byte to handle it later, and
            // return the chunk we have saved up to now.
            storeByte(midiByte);
            return MIDIReadEvent::SYSEX_CHUNK;
        }
        // Enough space is available in buffer, store the end byte and
        // terminate the message.
        addSysExByte(uint8_t(MIDIMessageType::SysExEnd));
        endSysEx();
        currentHeader = 0;
        runningHeader = 0;
        return MIDIReadEvent::SYSEX_MESSAGE;
    } else
#endif
    {
        // Tune Request is a special System Common message of 1 byte.
        if (midiByte == uint8_t(MIDIMessageType::TuneRequest)) {
            midimsg.header = midiByte;
            midimsg.data1 = 0;
            midimsg.data2 = 0;
            if (sysCommonCancelsRunningStatus)
                runningHeader = 0;
            currentHeader = 0;
            return MIDIReadEvent::SYSCOMMON_MESSAGE;
        }
#if !IGNORE_SYSEX
        // If the new status byte is a SysExStart, reset the SysEx buffer
        // and store the start byte.
        else if (midiByte == uint8_t(MIDIMessageType::SysExStart)) {
            startSysEx();
            addSysExByte(uint8_t(MIDIMessageType::SysExStart));
            runningHeader = 0;
            currentHeader = midiByte;
            return MIDIReadEvent::NO_MESSAGE;
        }
        // This should already have been handled by the if (untermSysEx) above.
        else if (midiByte == uint8_t(MIDIMessageType::SysExEnd)) {
            DEBUGREF(F("Unexpected SysEx End"));
            return MIDIReadEvent::NO_MESSAGE;
        }
#endif
        // Otherwise, start a System Common or Channel message.
        else {
            // Save the newly received status byte.
            currentHeader = midiByte;
            // A new message starts, so we haven't received the second byte
            // yet.
            thirdByte = false;
            return MIDIReadEvent::NO_MESSAGE;
        }
    }
}

/*
 * Relevant sources about MIDI running status:
 *  - MIDI 1.0 Detailed Specification: A-2 (pdf p.65):
 *    “cleared when a System Exclusive or Common status message is received”
 *  - BLE-MIDI: p.4 (pdf p.7):
 *    “System Common and System Real-Time messages do not cancel Running Status”
 */

MIDIReadEvent SerialMIDI_Parser::handleStatus(uint8_t midiByte) {
    // If it's a Real-Time message
    if (midiByte >= uint8_t(MIDIMessageType::TimingClock)) {
        return handleRealTime(midiByte);
    }
    // Normal header (channel message, system exclusive, system common):
    else {
        return handleNonRealTimeStatus(midiByte);
    }
}

MIDIReadEvent SerialMIDI_Parser::handleData(uint8_t midiByte) {
    if (currentHeader == 0) {
        // If we didn't receive a header, we can't do anything with this data
        if (runningHeader == 0) {
            DEBUGREF(F("Data byte ignored"));
            return MIDIReadEvent::NO_MESSAGE;
        }
        // If we have an active running status, use that as the current header
        currentHeader = runningHeader;
    }

    midimsg.header = currentHeader;

    // If this is the third byte of three (second data byte)
    if (thirdByte) {
        // If it's a channel message
        if (midimsg.hasValidChannelMessageHeader()) {
            midimsg.data2 = midiByte;
            // Next byte is either a header or the first data byte of the next
            // message, so clear the thirdByte flag
            thirdByte = false;
            runningHeader = midimsg.header;
            currentHeader = 0;
            return MIDIReadEvent::CHANNEL_MESSAGE;
        }
        // If it's a system common message
        else if (midimsg.hasValidSystemCommonHeader()) {
            midimsg.data2 = midiByte;
            thirdByte = false;
            if (sysCommonCancelsRunningStatus)
                runningHeader = 0;
            currentHeader = 0;
            return MIDIReadEvent::SYSCOMMON_MESSAGE;
        }
    }

    // If this is not the third byte of three, it's either the second byte
    // (first data byte) of a channel or system common message,
    // or a SysEx data byte

    // If it's a channel message
    else if (midimsg.hasValidChannelMessageHeader()) {
        // If it's a channel message with two data bytes
        if (ChannelMessage(midimsg).hasTwoDataBytes()) {
            midimsg.data1 = midiByte;
            // We've received the second byte, expect the third byte next
            thirdByte = true;
            return MIDIReadEvent::NO_MESSAGE;
        }
        // If it's a channel message with one data byte
        else {
            midimsg.data1 = midiByte;
            midimsg.data2 = 0;
            runningHeader = midimsg.header;
            currentHeader = 0;
            // The message is finished
            return MIDIReadEvent::CHANNEL_MESSAGE;
        }
    }

    // If it's a system common message
    else if (midimsg.hasValidSystemCommonHeader()) {
        // If it's a system common message with two data bytes
        if (SysCommonMessage(midimsg).getNumberOfDataBytes() == 2) {
            midimsg.data1 = midiByte;
            // We've received the second byte, expect the third byte next
            thirdByte = true;
            return MIDIReadEvent::NO_MESSAGE;
        }
        // If it's a system common message with one data byte
        else if (SysCommonMessage(midimsg).getNumberOfDataBytes() == 1) {
            midimsg.data1 = midiByte;
            midimsg.data2 = 0;
            if (sysCommonCancelsRunningStatus)
                runningHeader = 0;
            currentHeader = 0;
            // The message is finished
            return MIDIReadEvent::SYSCOMMON_MESSAGE;
        }
    }

    // Otherwise, it's not a channel message

#if !IGNORE_SYSEX
    // If we're receiving a SysEx message, it's a SysEx data byte
    else if (currentHeader == uint8_t(MIDIMessageType::SysExStart)) {
        // Check if the SysEx buffer has enough space to store the data
        if (!hasSysExSpace()) {
            storeByte(midiByte); // Remember to add it next time
            return MIDIReadEvent::SYSEX_CHUNK;
        }

        addSysExByte(midiByte);
        return MIDIReadEvent::NO_MESSAGE;
    }
#endif // IGNORE_SYSEX

    DEBUGREF(F("Data byte after invalid header")); // LCOV_EXCL_LINE
    runningHeader = 0;                             // LCOV_EXCL_LINE
    currentHeader = 0;                             // LCOV_EXCL_LINE
    return MIDIReadEvent::NO_MESSAGE;              // LCOV_EXCL_LINE
}

MIDIReadEvent SerialMIDI_Parser::feed(uint8_t midiByte) {
    // DEBUGREF(hex << NAMEDVALUE(midiByte) << dec);

    // If it's a status byte (first byte of a message)
    if (isStatus(midiByte)) {
        return handleStatus(midiByte);
    }
    // If it's a data byte
    else {
        return handleData(midiByte);
    }
}

MIDIReadEvent SerialMIDI_Parser::resume() {
    if (!hasStoredByte())
        return MIDIReadEvent::NO_MESSAGE;

    uint8_t midiByte = popStoredByte();

#if !IGNORE_SYSEX
    // If a SysEx message was in progress
    if (currentHeader == uint8_t(MIDIMessageType::SysExStart)) {
        // Reset the buffer for the next chunk
        startSysEx();
    }
#endif

    return feed(midiByte);
}

END_CS_NAMESPACE