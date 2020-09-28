#include "USBMIDI_Parser.hpp"
#include <Settings/SettingsWrapper.hpp>

BEGIN_CS_NAMESPACE

// https://usb.org/sites/default/files/midi10.pdf
MIDIReadEvent USBMIDI_Parser::parse(uint8_t *packet) {
    DEBUG("MIDIUSB packet:\t" << hex << packet[0] << ' ' << packet[1] << ' '
                              << packet[2] << ' ' << packet[3] << dec);
    // MIDI USB cable number
    uint8_t cable = packet[0] >> 4;
    // MIDI USB code index number
    MIDICodeIndexNumber CIN = static_cast<MIDICodeIndexNumber>(packet[0] & 0xF);

    // SysEx constants
    constexpr uint8_t SysExEnd =
        static_cast<uint8_t>(MIDIMessageType::SYSEX_END);
    constexpr uint8_t SysExStart =
        static_cast<uint8_t>(MIDIMessageType::SYSEX_START);

    // Main parser logic
    // =========================================================================

    // ------------------- Channel message (2 or 3 bytes) ------------------- //

    if (CIN >= MIDICodeIndexNumber::NOTE_OFF &&
        CIN <= MIDICodeIndexNumber::PITCH_BEND) {
        // 2- or 3-byte MIDI channel message

        // uint8_t type = packet[1] & 0xF0;
        // if (CIN != type) // invalid MIDI USB packet
        //    return NO_MESSAGE;
        midimsg.header = packet[1];
        midimsg.data1 = packet[2];
        midimsg.data2 = packet[3];
        midimsg.cable = cable;
        return MIDIReadEvent::CHANNEL_MESSAGE;
    }

    // ---------------- SysEx starts or continues (3 bytes) ----------------- //

#if !IGNORE_SYSEX
    else if (CIN == MIDICodeIndexNumber::SYSEX_START_CONT) {
        // SysEx starts or continues (3 bytes)
        if (packet[1] == SysExStart)
            startSysEx(cable); // start a new message
                            // (overwrite previous unfinished message)
        else if (!receivingSysEx(cable)) { // If we haven't received a SysExStart
            DEBUGREF(F("Error: No SysExStart received"));
            return MIDIReadEvent::NO_MESSAGE; // ignore the data
        }
        addSysExByte(cable, packet[1]) &&     // add three data bytes to buffer
            addSysExByte(cable, packet[2]) && //
            addSysExByte(cable, packet[3]);
        return MIDIReadEvent::NO_MESSAGE; // SysEx is not finished yet
    }

    // --------------- SysEx ends with following single byte ---------------- //

    else if (CIN == MIDICodeIndexNumber::SYSEX_END_1B) {
        // SysEx ends with following single byte
        // (or Single-byte System Common Message, not implemented)
        if (packet[1] != SysExEnd) {
            // System Common (not implemented)
            return MIDIReadEvent::NO_MESSAGE;
        } else if (!receivingSysEx(cable)) { // If we haven't received a SysExStart
            DEBUGFN(F("Error: No SysExStart received"));
            return MIDIReadEvent::NO_MESSAGE; // ignore the data
        }
        if (addSysExByte(cable, packet[1])) {
            endSysEx(cable);
            return MIDIReadEvent::SYSEX_MESSAGE;
        } else {
            return MIDIReadEvent::NO_MESSAGE; // Buffer full, ignore message
        }
    }

    // ---------------- SysEx ends with following two bytes ----------------- //

    else if (CIN == MIDICodeIndexNumber::SYSEX_END_2B) {
        // SysEx ends with following two bytes
        if (packet[1] == SysExStart)
            startSysEx(cable); // start a new message
        // (overwrite previous unfinished message)
        else if (!receivingSysEx(cable)) { // If we haven't received a SysExStart
            DEBUGFN(F("Error: No SysExStart received"));
            return MIDIReadEvent::NO_MESSAGE; // ignore the data
        }
        if ( // add two data bytes to buffer
            addSysExByte(cable, packet[1]) && addSysExByte(cable, SysExEnd)) {
            endSysEx(cable);
            return MIDIReadEvent::SYSEX_MESSAGE;
        } else
            return MIDIReadEvent::NO_MESSAGE; // Buffer full, ignore message
    }

    // --------------- SysEx ends with following three bytes ---------------- //

    else if (CIN == MIDICodeIndexNumber::SYSEX_END_3B) {
        // SysEx ends with following three bytes
        if (packet[1] == SysExStart)
            startSysEx(cable); // start a new message
                            // (overwrite previous unfinished message)
        else if (!receivingSysEx(cable)) { // If we haven't received a SysExStart
            DEBUGFN(F("Error: No SysExStart received"));
            return MIDIReadEvent::NO_MESSAGE; // ignore the data
        }
        if (                               // add three data bytes to buffer
            addSysExByte(cable, packet[1]) && //
            addSysExByte(cable, packet[2]) && //
            addSysExByte(cable, SysExEnd)) {
            endSysEx(cable);
            return MIDIReadEvent::SYSEX_MESSAGE;
        } else {
            return MIDIReadEvent::NO_MESSAGE; // Buffer full, ignore message
        }
    }
#endif // IGNORE_SYSEX

    else if (CIN == MIDICodeIndexNumber::MISC_FUNCTION_CODES) {
        // Miscellaneous function codes. Reserved for future extensions.
        // (not implemented)
    } else if (CIN == MIDICodeIndexNumber::CABLE_EVENTS) {
        // Cable events. Reserved for future expansion.
        // (not implemented)
    } else if (CIN == MIDICodeIndexNumber::SYSTEM_COMMON_2B) {
        // Two-byte System Common message
        // (not implemented)
    } else if (CIN == MIDICodeIndexNumber::SYSTEM_COMMON_3B) {
        // Three-byte System Common message
        // (not implemented)
    }

    // ---------------------- Single byte (real-time) ----------------------- //

    else if (CIN == MIDICodeIndexNumber::SINGLE_BYTE) {
        // Single Byte
        rtmsg.message = packet[1];
        rtmsg.cable = cable;
        return MIDIReadEvent::REALTIME_MESSAGE;
    }

    return MIDIReadEvent::NO_MESSAGE;
}

END_CS_NAMESPACE