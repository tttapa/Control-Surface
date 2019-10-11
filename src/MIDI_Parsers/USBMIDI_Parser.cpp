#include "USBMIDI_Parser.hpp"
#include <Settings/SettingsWrapper.hpp>

BEGIN_CS_NAMESPACE

// http://www.usb.org/developers/docs/devclass_docs/midi10.pdf
MIDI_read_t USBMIDI_Parser::parse(uint8_t *packet) {
    DEBUG("MIDIUSB packet:\t" << hex << packet[0] << ' ' << packet[1] << ' '
                              << packet[2] << ' ' << packet[3] << dec);
    this->CN = (uint8_t)packet[0] >> 4;
    uint8_t CIN = (uint8_t)packet[0] << 4; // MIDI USB code index number

    if (CIN >= NOTE_OFF && CIN <= PITCH_BEND) {
        // 2- or 3-byte MIDI event

        // uint8_t type = packet[1] & 0xF0;
        // if (CIN != type) // invalid MIDI USB packet
        //    return NO_MESSAGE;
        midimsg.header = packet[1];
        midimsg.data1 = packet[2];
        midimsg.data2 = packet[3];
        midimsg.CN = this->CN;
        return CHANNEL_MESSAGE;
    }

#if !IGNORE_SYSEX
    else if (CIN == 0x40) {
        // SysEx starts or continues (3 bytes)
        if (packet[1] == SysExStart)
            startSysEx(CN); // start a new message
                            // (overwrite previous unfinished message)
        else if (!receivingSysEx(CN)) { // If we haven't received a SysExStart
            DEBUGREF(F("Error: No SysExStart received"));
            return NO_MESSAGE; // ignore the data
        }
        addSysExByte(CN, packet[1]) && // add three data bytes to buffer
            addSysExByte(CN, packet[2]) && addSysExByte(CN, packet[3]);
        return NO_MESSAGE; // SysEx is not finished yet
    }

    else if (CIN == 0x50) {
        // SysEx ends with following single byte
        // (or Single-byte System Common Message, not implemented)
        if (packet[1] != SysExEnd) { // System Common (not implemented)
            return NO_MESSAGE;
        } else if (!receivingSysEx(CN)) { // If we haven't received a SysExStart
            DEBUGFN(F("Error: No SysExStart received"));
            return NO_MESSAGE; // ignore the data
        }
        if (addSysExByte(CN, SysExEnd)) {
            endSysEx(CN);
            return SYSEX_MESSAGE;
        } else
            return NO_MESSAGE; // Buffer full, ignore message
    }

    else if (CIN == 0x60) {
        // SysEx ends with following two bytes
        if (packet[1] == SysExStart)
            startSysEx(CN); // start a new message
        // (overwrite previous unfinished message)
        else if (!receivingSysEx(CN)) { // If we haven't received a SysExStart
            DEBUGFN(F("Error: No SysExStart received"));
            return NO_MESSAGE; // ignore the data
        }
        if ( // add two data bytes to buffer
            addSysExByte(CN, packet[1]) && addSysExByte(CN, SysExEnd)) {
            endSysEx(CN);
            return SYSEX_MESSAGE;
        } else
            return NO_MESSAGE; // Buffer full, ignore message
    }

    else if (CIN == 0x70) {
        // SysEx ends with following three bytes
        if (packet[1] == SysExStart)
            startSysEx(CN); // start a new message
                            // (overwrite previous unfinished message)
        else if (!receivingSysEx(CN)) { // If we haven't received a SysExStart
            DEBUGFN(F("Error: No SysExStart received"));
            return NO_MESSAGE; // ignore the data
        }
        if ( // add three data bytes to buffer
            addSysExByte(CN, packet[1]) && addSysExByte(CN, packet[2]) &&
            addSysExByte(CN, SysExEnd)) {
            endSysEx(CN);
            return SYSEX_MESSAGE;
        } else
            return NO_MESSAGE; // Buffer full, ignore message
    }
#endif // IGNORE_SYSEX

    /*
    else if (CIN == 0x00) // Miscellaneous function codes. Reserved for future extensions. (not implemented)
      ;
    else if (CIN == 0x10) // Cable events. Reserved for future expansion. (not implemented)
      ;
    else if (CIN == 0x20) // Two-byte System Common message (not implemented)
      ;
    else if (CIN == 0x30) // Three-byte System Common message (not implemented)
      ;
  */
    else if (CIN == 0xF0) // Single Byte
        return static_cast<MIDI_read_t>(packet[1]);

    return NO_MESSAGE;
}

END_CS_NAMESPACE