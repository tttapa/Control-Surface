#include "SerialMIDI_Parser.hpp"

BEGIN_CS_NAMESPACE

MIDIReadEvent SerialMIDI_Parser::handleRealTime(uint8_t midiByte) {
    rtmsg.message = midiByte;
    return MIDIReadEvent::REALTIME_MESSAGE;
}

MIDIReadEvent SerialMIDI_Parser::handleStatus(uint8_t midiByte) {
    // If it's a Real-Time message
    if (midiByte >= uint8_t(MIDIMessageType::TIMING_CLOCK)) {
        return handleRealTime(midiByte);
    }

    // Normal header (channel message, system exclusive, system common)
    else {
        // If a SysEx message was being received, and now we receive
        // another status byte, remember to correctly terminate the SysEx
        // message later
        bool unterminatedSysEx =
            runningHeader == uint8_t(MIDIMessageType::SYSEX_START);

        // Tune Request is a special System Common message of 1 byte
        if (midiByte == uint8_t(MIDIMessageType::TUNE_REQUEST)) {
            if (unterminatedSysEx) {
                // If we were previously receiving a SysEx message, this Tune
                // Request should terminate that SysEx message first.
                // We cannot return both the SysEx message and the Tune Request
                // at the same time, so store the Tune Request for later.
                storeByte(midiByte);
            } else {
                midimsg.header = midiByte;
                midimsg.data1 = 0;
                midimsg.data2 = 0;
                runningHeader = 0;
                return MIDIReadEvent::SYSCOMMON_MESSAGE;
            }
        }

#if !IGNORE_SYSEX
        // If the new status byte terminates the SysEx message, try to finish
        // the message (if buffer size allows it)
        if (unterminatedSysEx) {
            // Check if the SysEx buffer has enough space to store the end byte
            if (!hasSysExSpace()) {
                storeByte(midiByte);
                return MIDIReadEvent::SYSEX_CHUNK;
            }

            // Enough space available in buffer, store the end byte and
            // terminate the message
            addSysExByte(uint8_t(MIDIMessageType::SYSEX_END));
            endSysEx();

            // If the previous SysEx message was terminated by the SysExStart
            // byte of next message, we have to return the previous message now
            // and remember to start the next message in the next iteration
            if (midiByte == uint8_t(MIDIMessageType::SYSEX_START)) {
                storeByte(midiByte);
                runningHeader = 0;
                return MIDIReadEvent::SYSEX_MESSAGE;
            }
        }

        // If we don't have to handle the unterminated SysEx message, and the
        // new byte is a SysExStart, reset the SysEx buffer and store the start
        // byte
        else if (midiByte == uint8_t(MIDIMessageType::SYSEX_START)) {
            startSysEx();
            addSysExByte(uint8_t(MIDIMessageType::SYSEX_START));
        }

        // Save the newly received status byte
        runningHeader = midiByte;
        // A new message starts, so we haven't received the second byte yet
        thirdByte = false;

        return unterminatedSysEx ? MIDIReadEvent::SYSEX_MESSAGE
                                 : MIDIReadEvent::NO_MESSAGE;
#else
        (void)unterminatedSysEx;
        // Save the newly received status byte
        runningHeader = midiByte;
        // A new message starts, so we haven't received the second byte yet
        thirdByte = false;

        return MIDIReadEvent::NO_MESSAGE;
#endif // IGNORE_SYSEX
    }
}

MIDIReadEvent SerialMIDI_Parser::handleData(uint8_t midiByte) {
    midimsg.header = runningHeader;

    // If this is the third byte of three (second data byte)
    if (thirdByte) {
        // If it's a channel message
        if (midimsg.hasValidChannelMessageHeader()) {
            midimsg.data2 = midiByte;
            // Next byte is either a header or the first data byte of the next
            // message, so clear the thirdByte flag
            thirdByte = false;
            return MIDIReadEvent::CHANNEL_MESSAGE;
        }
        // If it's a system common message
        else if (midimsg.hasValidSystemCommonHeader()) {
            midimsg.data2 = midiByte;
            thirdByte = false;
            runningHeader = 0;
            return MIDIReadEvent::SYSCOMMON_MESSAGE;
        } else {
            // If this happens, something's wrong with the parser logic
            ERROR(F("Unexpected third byte"), 0x3333); // LCOV_EXCL_LINE
            return MIDIReadEvent::NO_MESSAGE;          // LCOV_EXCL_LINE
        }
    }

    // If this is not the third byte of three, it's either the second byte
    // (first data byte) of a channel message, or a SysEx data byte

    // If the previous SysEx was complete, but no other headers were received
    else if (runningHeader == uint8_t(MIDIMessageType::SYSEX_END)) {
        DEBUGREF(F("Unexpected data after SysExEnd"));
        return MIDIReadEvent::NO_MESSAGE;
    }

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
            // The message is finished
            return MIDIReadEvent::CHANNEL_MESSAGE;
        }
    }

    // If it's a system common message
    else if (midimsg.hasValidSystemCommonHeader()) {
        // If it's a channel message with two data bytes
        if (SysCommonMessage(midimsg).getNumberOfDataBytes() == 2) {
            midimsg.data1 = midiByte;
            // We've received the second byte, expect the third byte next
            thirdByte = true;
            return MIDIReadEvent::NO_MESSAGE;
        }
        // If it's a channel message with one data byte
        else if (SysCommonMessage(midimsg).getNumberOfDataBytes() == 1) {
            midimsg.data1 = midiByte;
            midimsg.data2 = 0;
            runningHeader = 0;
            // The message is finished
            return MIDIReadEvent::SYSCOMMON_MESSAGE;
        } else {
            DEBUGREF(F("Unexpected data byte"));
            return MIDIReadEvent::NO_MESSAGE;
        }
    }

    // Otherwise, it's not a channel message

#if !IGNORE_SYSEX
    // If we're receiving a SysEx message, it's a SysEx data byte
    else if (runningHeader == uint8_t(MIDIMessageType::SYSEX_START)) {
        // Check if the SysEx buffer has enough space to store the data
        if (!hasSysExSpace()) {
            storeByte(midiByte); // Remember to add it next time
            return MIDIReadEvent::SYSEX_CHUNK;
        }

        addSysExByte(midiByte);
        return MIDIReadEvent::NO_MESSAGE;
    }
#endif // IGNORE_SYSEX

    // We didn't receive a valid header, so we can't do anything with this data
    else {
        DEBUGREF(F("Data byte ignored"));
        return MIDIReadEvent::NO_MESSAGE;
    }
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

    // If a SysEx message was in progress
    bool receivingSysEx =
        runningHeader == uint8_t(MIDIMessageType::SYSEX_START);

    if (receivingSysEx) {
        // Reset the buffer for the next chunk
        startSysEx();
    }

    return feed(midiByte);
}

END_CS_NAMESPACE