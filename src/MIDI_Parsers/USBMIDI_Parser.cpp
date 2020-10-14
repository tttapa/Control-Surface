#include "USBMIDI_Parser.hpp"
#include <Settings/SettingsWrapper.hpp>

BEGIN_CS_NAMESPACE

MIDIReadEvent USBMIDI_Parser::handleChannelMessage(const uint8_t *packet,
                                                   uint8_t cable) {
    midimsg.header = packet[1];
    midimsg.data1 = packet[2];
    midimsg.data2 = packet[3];
    midimsg.cable = cable;
    return MIDIReadEvent::CHANNEL_MESSAGE;
}

MIDIReadEvent USBMIDI_Parser::handleSysExStartContinue(const uint8_t *packet,
                                                       uint8_t cable) {
    // If this is a SysEx start packet
    if (packet[1] == uint8_t(MIDIMessageType::SYSEX_START)) {
        startSysEx(cable); // start a new message
                           // (overwrites previous unfinished message)
    }
    // If we haven't received a SysExStart
    else if (!receivingSysEx(cable)) {
        DEBUGREF(F("Error: No SysExStart received"));
        return MIDIReadEvent::NO_MESSAGE; // ignore the data
    }

    // Check if the SysEx buffer has enough space to store the data
    if (!hasSysExSpace(cable, 3)) {
        storePacket(packet);
        return MIDIReadEvent::SYSEX_CHUNK;
    }

    // Enough space available in buffer, store the data
    addSysExBytes(cable, &packet[1], 3);
    return MIDIReadEvent::NO_MESSAGE; // SysEx is not finished yet
}

MIDIReadEvent USBMIDI_Parser::handleSysExEnd1B(const uint8_t *packet,
                                               uint8_t cable) {
    // Single-byte System Common Message (not implemented)
    if (packet[1] != uint8_t(MIDIMessageType::SYSEX_END)) {
        // System Common (not implemented)
        return MIDIReadEvent::NO_MESSAGE;
    }

    // SysEx ends with following single byte
    else {
        // If we haven't received a SysExStart
        if (!receivingSysEx(cable)) {
            DEBUGFN(F("Error: No SysExStart received"));
            return MIDIReadEvent::NO_MESSAGE; // ignore the data
        }

        // Check if the SysEx buffer has enough space to store the end byte
        if (!hasSysExSpace(cable, 1)) {
            storePacket(packet);
            return MIDIReadEvent::SYSEX_CHUNK;
        }

        // Enough space available in buffer, finish the message
        addSysExByte(cable, packet[1]);
        endSysEx(cable);
        return MIDIReadEvent::SYSEX_MESSAGE;
    }
}

template <uint8_t NumBytes>
MIDIReadEvent USBMIDI_Parser::handleSysExEnd(const uint8_t *packet,
                                             uint8_t cable) {
    static_assert(NumBytes == 2 || NumBytes == 3,
                  "Only 2- or 3-byte SysEx packets are supported");

    // This could be the a very short SysEx message that starts and ends with
    // this packet
    if (packet[1] == uint8_t(MIDIMessageType::SYSEX_START)) {
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
        return MIDIReadEvent::SYSEX_CHUNK; // Buffer full
    }

    // Enough space available in buffer, finish the message
    addSysExBytes(cable, &packet[1], NumBytes);
    endSysEx(cable);
    return MIDIReadEvent::SYSEX_MESSAGE;
}

// Single Byte
MIDIReadEvent USBMIDI_Parser::handleSingleByte(const uint8_t *packet,
                                               uint8_t cable) {
    rtmsg.message = packet[1];
    rtmsg.cable = cable;
    return MIDIReadEvent::REALTIME_MESSAGE;
}

// https://usb.org/sites/default/files/midi10.pdf
MIDIReadEvent USBMIDI_Parser::parse(const uint8_t *packet) {
    // DEBUG("MIDIUSB packet:\t" << hex << packet[0] << ' ' << packet[1] << ' '
    //                           << packet[2] << ' ' << packet[3] << dec);

    // MIDI USB cable number
    uint8_t cable = packet[0] >> 4;
    // MIDI USB code index number
    MIDICodeIndexNumber CIN = static_cast<MIDICodeIndexNumber>(packet[0] & 0xF);

    switch (CIN) {
        case MIDICodeIndexNumber::MISC_FUNCTION_CODES: break;
        case MIDICodeIndexNumber::CABLE_EVENTS: break;
        case MIDICodeIndexNumber::SYSTEM_COMMON_2B: break;
        case MIDICodeIndexNumber::SYSTEM_COMMON_3B: break;

        case MIDICodeIndexNumber::SYSEX_START_CONT:
            return handleSysExStartContinue(packet, cable);

        case MIDICodeIndexNumber::SYSEX_END_1B:
            return handleSysExEnd1B(packet, cable);
        case MIDICodeIndexNumber::SYSEX_END_2B:
            return handleSysExEnd<2>(packet, cable);
        case MIDICodeIndexNumber::SYSEX_END_3B:
            return handleSysExEnd<3>(packet, cable);

        case MIDICodeIndexNumber::NOTE_OFF:         // fallthrough
        case MIDICodeIndexNumber::NOTE_ON:          // fallthrough
        case MIDICodeIndexNumber::KEY_PRESSURE:     // fallthrough
        case MIDICodeIndexNumber::CONTROL_CHANGE:   // fallthrough
        case MIDICodeIndexNumber::PROGRAM_CHANGE:   // fallthrough
        case MIDICodeIndexNumber::CHANNEL_PRESSURE: // fallthrough
        case MIDICodeIndexNumber::PITCH_BEND:
            return handleChannelMessage(packet, cable);

        case MIDICodeIndexNumber::SINGLE_BYTE:
            return handleSingleByte(packet, cable);

        default: break;
    }

    return MIDIReadEvent::NO_MESSAGE;
}

END_CS_NAMESPACE