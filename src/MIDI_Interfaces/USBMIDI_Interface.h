#ifndef USBMIDI_INTERFACE_H_
#define USBMIDI_INTERFACE_H_

#include "MIDI_Interface.h"
#include "../Helpers/StreamOut.h"

#ifdef CORE_TEENSY
#include <usb_dev.h>
#endif

#if defined(USBCON) && !defined(CORE_TEENSY) // If the main MCU has a USB connection but is not a Teensy
#include "MIDIUSB.h"
#endif

#if defined(USBCON) || defined(CORE_TEENSY) // If the main MCU has a USB connection (or is a Teensy)
class USBMIDI_Interface : public MIDI_Interface
{
  public:
    USBMIDI_Interface()
    {
        ;
    }

  protected:
    void sendImpl(uint8_t m, uint8_t c, uint8_t d1, uint8_t d2)
    {
#if defined(CORE_TEENSY) // If it's a Teensy board
        usb_midi_write_packed((m >> 4) | ((m | c) << 8) | (d1 << 16) | (d2 << 24));
#elif defined(USBCON) // If the main MCU has a USB connection but is not a Teensy
        midiEventPacket_t msg = {m >> 4, m | c, d1, d2};
        MidiUSB.sendMIDI(msg);
        MidiUSB.flush();
#endif
    }
    void sendImpl(uint8_t m, uint8_t c, uint8_t d1)
    {
        sendImpl(m, c, d1, 0);
    }

#ifndef NO_MIDI_INPUT

    MIDI_read_t parseUSBMIDIpacket(uint8_t *packet)
    {
#ifdef DEBUG
        DEBUG << "\t\t\t\t\t\tMIDIUSB packet:\t"
              << hex << packet[0] << ' ' << packet[1] << ' ' << packet[2] << ' ' << packet[3] << dec << endl;
#endif

        uint8_t CIN = (uint8_t)packet[0] << 4; // MIDI USB cable index number

        if (CIN >= NOTE_OFF && CIN <= PITCH_BEND) // 2- or 3-byte MIDI event
        {
            midimsg.header = packet[1];
            uint8_t type = midimsg.header & 0xF0;

            // if (CIN != type) // invalid MIDI USB packet
            //    return true;

            midimsg.data1 = packet[2];
            midimsg.data2 = packet[3];
            return CHANNEL_MESSAGE;
        }
#ifndef IGNORE_SYSEX
        else if (CIN == 0x40) // SysEx starts or continues (3 bytes)
        {
            if (packet[1] == SysExStart)
            {
                startSysEx(); // start a new message (overwrite previous unfinished message)
                addSysExByte(packet[2]);
                addSysExByte(packet[3]);
            }
            else if (SysExLength == 0) // If we haven't received a SysExStart
            {
#ifdef DEBUG
                Serial.println("No SysExStart received");
#endif
                ; // ignore the data
            }
            else // SysEx continues
            {
                addSysExByte(packet[1]); // add three data bytes to buffer
                addSysExByte(packet[2]);
                addSysExByte(packet[3]);
            }
        }
        else if (CIN == 0x50) // SysEx ends with following single byte (or Single-byte System Common Message, not implemented)
        {
            if (packet[1] == SysExEnd) // System Common (not implemented)
            {
                ;
            }
            else if (SysExLength == 0) // If we haven't received a SysExStart
            {
#ifdef DEBUG
                Serial.println("No SysExStart received");
#endif
            }
            else
            {
                addSysExByte(SysExEnd);
                return SYSEX_MESSAGE;
            }
        }
        else if (CIN == 0x60) // SysEx ends with following two bytes
        {
            if (SysExLength == 0) // If we haven't received a SysExStart
            {
                ; // ignore the data
            }
            else
            {
                addSysExByte(packet[1]); // add two data bytes to buffer
                addSysExByte(SysExEnd);
                return SYSEX_MESSAGE;
            }
        }
        else if (CIN == 0x70) // SysEx ends with following three bytes
        {
            if (SysExLength == 0) // If we haven't received a SysExStart
            {
                ; // ignore the data
            }
            else
            {
                addSysExByte(packet[1]); // add three data bytes to buffer
                addSysExByte(packet[2]);
                addSysExByte(SysExEnd);
                return SYSEX_MESSAGE;
            }
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
        else if (CIN == 0xF0) // Single Byte (not implemented)
            ;
        */

        return NO_MESSAGE; // return, increment USB buffer pointer
    }

  public:
    MIDI_read_t read()
    {
#if defined(CORE_TEENSY) // If it's a Teensy board
        while (1)
        {
            if (rx_packet == nullptr) // If there's no previous packet
            {
                if (!usb_configuration) // Check USB configuration
                    return NO_MESSAGE;
                rx_packet = usb_rx(MIDI_RX_ENDPOINT); // Read a new packet from the USB buffer
                if (rx_packet == nullptr)             // If there's no new packet, return
                    return NO_MESSAGE;
                if (rx_packet->len < 4) // If the lenght is less than 4, it's not a valid MIDI USB packet
                {
                    usb_free(rx_packet); // Free the packet
                    rx_packet = nullptr; // Read new packet next time around
                    continue;
                }
            }

            size_t index = rx_packet->index;

            uint8_t *data = rx_packet->buf + index; // A pointer to this packet

            MIDI_read_t parseResult = parseUSBMIDIpacket(data);
            if (parseResult != NO_MESSAGE)
                return parseResult;

            index += 4;
            if (index < rx_packet->len) // If the packet is longer than 4 bytes
            {
                rx_packet->index = index; // Change the index and read the next four bytes on next read
            }
            else // If the packet was only 4 bytes long
            {
                usb_free(rx_packet);                  // Free the packet
                rx_packet = usb_rx(MIDI_RX_ENDPOINT); // Read the next packet
            }
        }

#elif defined(USBCON) // If the main MCU has a USB connection but is not a Teensy
        while (1)
        {
            rx_packet = *(uint32_t *)&MidiUSB.read();
            if (rx_packet == 0)
                return NO_MESSAGE;
            MIDI_read_t parseResult = parseUSBMIDIpacket((uint8_t *)&rx_packet);
            if (parseResult != NO_MESSAGE)
                return parseResult;
        }
#endif
    }

  private:
#if defined(CORE_TEENSY)
    usb_packet_t *rx_packet = nullptr;
#elif defined(USBCON) // If the main MCU has a USB connection but is not a Teensy
    uint32_t rx_packet = 0;
#endif

#else                    // #ifndef NO_MIDI_INPUT

  public:
  /*
  TODO
    bool refresh() // Ignore MIDI input
    {
#if defined(CORE_TEENSY) // If it's a Teensy board
        if (!usb_configuration) // Check USB configuration
            return false;
        usb_packet_t *rx_packet = usb_rx(MIDI_RX_ENDPOINT); // Read a new packet from the USB buffer
        if (rx_packet == nullptr)                           // If there's no new packet, return
            return false;

        usb_free(rx_packet); // Free the packet
        return true;         // repeat
#elif defined(USBCON)    // If the main MCU has a USB connection but is not a Teensy
        return MidiUSB.read().header != 0; // if there's a packet to read, discard it, and read again next time
#endif
    }
    */

#endif // #ifndef NO_MIDI_INPUT
};

#else // If the main MCU doesn't have a USB connection

#include "SerialMIDI_Interface.h"

class USBMIDI_Interface : public USBSerialMIDI_Interface
{
  public:
    USBMIDI_Interface() : USBSerialMIDI_Interface(MIDI_BAUD){};
};

#endif

#endif // USBMIDI_INTERFACE_H_