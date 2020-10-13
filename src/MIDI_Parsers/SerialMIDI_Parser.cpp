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
            midimsg.header == uint8_t(MIDIMessageType::SYSEX_START);

        // Tune Request is a special System Common message of 1 byte
        if (midiByte == uint8_t(MIDIMessageType::TUNE_REQUEST)) {
            // Tune request (not implemented)
            // TODO: should I implement this as a Real-Time message?
            // (That might lead to problems when a SysEx message is
            // terminated by a Tune Request, see unterminatedSysEx below)
            // TODO: should I set midimsg.header in this case?
            if (unterminatedSysEx) {
                // If we were previously receiving a SysEx message, this Tune
                // Request should terminate that SysEx message first.
                // We cannot return both the SysEx message and the Tune Request
                // at the same time, so store the Tune Request for later.
                storeByte(midiByte);
            } else {
                // TODO: store SysCommon byte
                // return MIDIReadEvent::SYSTEM_COMMON_MESSAGE;
                return MIDIReadEvent::NO_MESSAGE;
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
                midimsg.header = 0;
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
        midimsg.header = midiByte;
        // A new message starts, so we haven't received the second byte yet
        thirdByte = false;

        return unterminatedSysEx ? MIDIReadEvent::SYSEX_MESSAGE
                                 : MIDIReadEvent::NO_MESSAGE;
#else
        (void)unterminatedSysEx;
        // Save the newly received status byte
        midimsg.header = midiByte;
        // A new message starts, so we haven't received the second byte yet
        thirdByte = false;

        return MIDIReadEvent::NO_MESSAGE;
#endif // IGNORE_SYSEX
    }
}

MIDIReadEvent SerialMIDI_Parser::handleData(uint8_t midiByte) {
    // If this is the third byte of three (second data byte)
    if (thirdByte) {
        midimsg.data2 = midiByte;
        // Next byte is either a header or the first data byte of the next
        // message, so clear the thirdByte flag
        thirdByte = false;
        return MIDIReadEvent::CHANNEL_MESSAGE;
    }

    // If this is not the third byte of three, it's either the second byte
    // (first data byte) of a channel message, or a SysEx data byte

    // If it's a channel message with two data bytes
    else if (midimsg.hasTwoDataBytes()) {
        midimsg.data1 = midiByte;
        // We've received the second byte, expect the third byte next
        thirdByte = true;
        return MIDIReadEvent::NO_MESSAGE;
    }

    // If it's a channel message with one data byte
    // (if it has a valid channel message header)
    else if (midimsg.hasValidHeader()) {
        midimsg.data1 = midiByte;
        // The message is finished
        return MIDIReadEvent::CHANNEL_MESSAGE;
    }

    // Otherwise, it's not a channel message

#if !IGNORE_SYSEX
    // If we're receiving a SysEx message, it's a SysEx data byte
    else if (midimsg.header == uint8_t(MIDIMessageType::SYSEX_START)) {
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
        midimsg.header == uint8_t(MIDIMessageType::SYSEX_START);

    if (receivingSysEx) {
        // Reset the buffer for the next chunk
        startSysEx();
    }

    return feed(midiByte);
}

END_CS_NAMESPACE