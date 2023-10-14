#include "USBMIDI_Parser.hpp"
#include <Settings/SettingsWrapper.hpp>

BEGIN_CS_NAMESPACE

MIDIReadEvent USBMIDI_Parser::handleChannelMessage(MIDIUSBPacket_t packet,
                                                   Cable cable) {
    midimsg.header = packet[1];
    midimsg.data1 = packet[2];
    midimsg.data2 = packet[3];
    midimsg.cable = cable;
    return MIDIReadEvent::CHANNEL_MESSAGE;
}

MIDIReadEvent USBMIDI_Parser::handleSysExStartCont(MIDIUSBPacket_t packet,
                                                   Cable cable) {
#if !IGNORE_SYSEX
    // If this is a SysEx start packet
    if (packet[1] == uint8_t(MIDIMessageType::SysExStart)) {
        startSysEx(cable); // start a new message
                           // (overwrites previous unfinished message)
    }
    // If we haven't received a SysExStart
    else if (!receivingSysEx(cable)) {
        DEBUGREF(F("No SysExStart received"));
        return MIDIReadEvent::NO_MESSAGE; // ignore the data
    }

    // Check if the SysEx buffer has enough space to store the data
    if (!hasSysExSpace(cable, 3)) {
        storePacket(packet);
        endSysExChunk(cable);
        return MIDIReadEvent::SYSEX_CHUNK;
    }

    // Enough space available in buffer, store the data
    addSysExBytes(cable, &packet[1], 3);
#else
    (void)packet;
    (void)cable;
#endif
    return MIDIReadEvent::NO_MESSAGE; // SysEx is not finished yet
}

template <uint8_t NumBytes>
MIDIReadEvent USBMIDI_Parser::handleSysExEnd(MIDIUSBPacket_t packet,
                                             Cable cable) {
    static_assert(NumBytes == 2 || NumBytes == 3,
                  "Only 2- or 3-byte SysEx packets are supported");

#if !IGNORE_SYSEX
    // This could be the a very short SysEx message that starts and ends with
    // this packet
    if (packet[1] == uint8_t(MIDIMessageType::SysExStart)) {
        startSysEx(cable); // start a new message
                           // (overwrites previous unfinished message)
    }
    // If we haven't received a SysExStart
    else if (!receivingSysEx(cable)) {
        DEBUGFN(F("No SysExStart received"));
        return MIDIReadEvent::NO_MESSAGE; // ignore the data
    }

    // Check if the SysEx buffer has enough space to store the end byte
    if (!hasSysExSpace(cable, NumBytes)) {
        storePacket(packet);
        endSysExChunk(cable);
        return MIDIReadEvent::SYSEX_CHUNK; // Buffer full
    }

    // Enough space available in buffer, finish the message
    addSysExBytes(cable, &packet[1], NumBytes);
    endSysEx(cable);
    return MIDIReadEvent::SYSEX_MESSAGE;
#else
    (void)packet;
    (void)cable;
    return MIDIReadEvent::NO_MESSAGE;
#endif
}

template <>
MIDIReadEvent USBMIDI_Parser::handleSysExEnd<1>(MIDIUSBPacket_t packet,
                                                Cable cable) {
    // Single-byte System Common Message
    if (packet[1] != uint8_t(MIDIMessageType::SysExEnd)) {
        // System Common (1 byte)
        midimsg.header = packet[1];
        midimsg.cable = cable;
        return MIDIReadEvent::SYSCOMMON_MESSAGE;
    }

#if !IGNORE_SYSEX
    // SysEx ends with following single byte
    else {
        // If we haven't received a SysExStart
        if (!receivingSysEx(cable)) {
            DEBUGREF(F("No SysExStart received"));
            return MIDIReadEvent::NO_MESSAGE; // ignore the data
        }

        // Check if the SysEx buffer has enough space to store the end byte
        if (!hasSysExSpace(cable, 1)) {
            storePacket(packet);
            endSysExChunk(cable);
            return MIDIReadEvent::SYSEX_CHUNK;
        }

        // Enough space available in buffer, finish the message
        addSysExByte(cable, packet[1]);
        endSysEx(cable);
        return MIDIReadEvent::SYSEX_MESSAGE;
    }
#else
    (void)packet;
    (void)cable;
    return MIDIReadEvent::NO_MESSAGE;
#endif
}

MIDIReadEvent USBMIDI_Parser::handleSysCommon(MIDIUSBPacket_t packet,
                                              Cable cable) {
    midimsg.header = packet[1];
    midimsg.data1 = packet[2];
    midimsg.data2 = packet[3];
    midimsg.cable = cable;
    return MIDIReadEvent::SYSCOMMON_MESSAGE;
}

// Single Byte
MIDIReadEvent USBMIDI_Parser::handleSingleByte(MIDIUSBPacket_t packet,
                                               Cable cable) {
    rtmsg.message = packet[1];
    rtmsg.cable = cable;
    return MIDIReadEvent::REALTIME_MESSAGE;
}

// https://usb.org/sites/default/files/midi10.pdf
MIDIReadEvent USBMIDI_Parser::feed(MIDIUSBPacket_t packet) {
    // DEBUG("MIDIUSB packet:\t" << hex << packet[0] << ' ' << packet[1] << ' '
    //                           << packet[2] << ' ' << packet[3] << dec);

    // MIDI USB cable number and code index number
    Cable cable = Cable(packet[0] >> 4);
    MIDICodeIndexNumber CIN = MIDICodeIndexNumber(packet[0] & 0xF);

    // Ignore all messages for cables that we don't have
    if (cable.getRaw() >= USB_MIDI_NUMBER_OF_CABLES)
        return MIDIReadEvent::NO_MESSAGE; // LCOV_EXCL_LINE

    using M = MIDICodeIndexNumber;
    switch (CIN) {
        case M::MiscFunctionCodes: break; // LCOV_EXCL_LINE
        case M::CableEvents: break;       // LCOV_EXCL_LINE
        case M::SystemCommon2B:           // fallthrough
        case M::SystemCommon3B: return handleSysCommon(packet, cable);
        case M::SysExStartCont: return handleSysExStartCont(packet, cable);
        case M::SysExEnd1B: return handleSysExEnd<1>(packet, cable);
        case M::SysExEnd2B: return handleSysExEnd<2>(packet, cable);
        case M::SysExEnd3B: return handleSysExEnd<3>(packet, cable);
        case M::NoteOff:         // fallthrough
        case M::NoteOn:          // fallthrough
        case M::KeyPressure:     // fallthrough
        case M::ControlChange:   // fallthrough
        case M::ProgramChange:   // fallthrough
        case M::ChannelPressure: // fallthrough
        case M::PitchBend: return handleChannelMessage(packet, cable);
        case M::SingleByte: return handleSingleByte(packet, cable);
        default: break; // LCOV_EXCL_LINE
    }

    return MIDIReadEvent::NO_MESSAGE; // LCOV_EXCL_LINE
}

MIDIReadEvent USBMIDI_Parser::resume() {
#if !IGNORE_SYSEX
    if (!hasStoredPacket())
        return MIDIReadEvent::NO_MESSAGE;

    MIDIUSBPacket_t packet = popStoredPacket();

    // If a SysEx message was in progress
    if (receivingSysEx(activeCable)) {
        // Reset the buffer for the next chunk
        startSysEx(activeCable);
    }

    return feed(packet);
#else
    return MIDIReadEvent::NO_MESSAGE;
#endif
}

END_CS_NAMESPACE