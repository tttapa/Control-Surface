#ifndef USBMIDI_INTERFACE_H_
#define USBMIDI_INTERFACE_H_

#include "MIDI_Interface.h"

#ifdef CORE_TEENSY
#include <usb_dev.h>
#include <usb_midi.h>
#endif

#if defined(USBCON) && !defined(CORE_TEENSY) // If the main MCU has a USB connection but is not a Teensy
#include "MIDIUSB.h"
#endif

#if defined(CORE_TEENSY) && !(defined(USB_MIDI_AUDIO_SERIAL) || defined(USB_MIDI) || defined(USB_MIDI_SERIAL))
#error Please select a MIDI option in the 'Tools > USB Type' menu.
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

    bool parseUSBMIDIpacket(uint8_t *packet)
    {
#ifdef DEBUG
        Serial.printf("\t\t\t\t\t\tMIDIUSB packet:\t%02X %02X %02X %02X\tSysEx length:\t%lu\r\n", packet[0], packet[1], packet[2], packet[3], SysExLength);
#endif

        uint8_t CIN = (uint8_t)packet[0] << 4; // MIDI USB cable index number

        if (CIN >= NOTE_OFF && CIN <= PITCH_BEND) // 2- or 3-byte MIDI event
        {
            uint8_t header = packet[1];
            uint8_t type = header & 0xF0;

            // if (CIN != type) // invalid MIDI USB packet
            //    return true;

            startMessage();                                        // start a new message (overwrite previous unfinished message)
            if (!addToMessage(header) || !addToMessage(packet[2])) // add header and data1 to buffer
                return false;                                      // if that fails, return, and don't increment USB buffer pointer
            if (CIN != PROGRAM_CHANGE && CIN != CHANNEL_PRESSURE)  // if it's a 3-byte MIDI event
                if (!addToMessage(packet[3]))                      // add data2 to buffer
                    return false;                                  // if that fails, return, and don't increment USB buffer pointer
            finishMessage();                                       // successfully added message to buffer, finish it
        }
#ifndef IGNORE_SYSEX
        else if (CIN == 0x40) // SysEx starts or continues (3 bytes)
        {
            if (packet[1] == SysExStart)
            {
                startMessage(); // start a new message (overwrite previous unfinished message)
                SysExLength = 0;
            }
            else if (SysExLength == 0) // If we haven't received a SysExStart
            {
#ifdef DEBUG
                Serial.println("No SysExStart received");
#endif
                return true; // ignore the data
            }

            SysExLength += 3;
            if (SysExLength > bufferSize) // SysEx is larger than the buffer
            {
#ifdef DEBUG
                Serial.println("SysEx is larger than buffer");
#endif
                startMessage(); // Discard message
                SysExLength = 0;
                return true;
            }
            if (!hasSpaceLeft(3)) // If there's no more free space in the buffer for three bytes
            {
                SysExLength -= 3;
                return false; // return, and don't increment USB buffer pointer
            }
            addToMessage(packet[1]); // add three data bytes to buffer
            addToMessage(packet[2]);
            addToMessage(packet[3]);
        }
        else if (CIN == 0x50) // SysEx ends with following single byte (or Single-byte System Common Message, not implemented)
        {
            if (packet[1] != SysExEnd) // System Common (not implemented)
                return true;

            if (SysExLength == 0) // If we haven't received a SysExStart
                return true;      // ignore the data

            SysExLength += 1;
            if (SysExLength > bufferSize) // SysEx is larger than the buffer
            {
#ifdef DEBUG
                Serial.println("SysEx is larger than buffer");
#endif
                startMessage(); // Discard message
                SysExLength = 0;
                return true;
            }
            if (!addToMessage(SysExEnd)) // add SysExEnd to buffer
            {
                SysExLength -= 1;
                return false; // if that fails, return, and don't increment USB buffer pointer
            }
            finishMessage(); // successfully added SysEx message to buffer, finish it
            SysExLength = 0;
        }
        else if (CIN == 0x60) // SysEx ends with following two bytes
        {
            if (SysExLength == 0) // If we haven't received a SysExStart
                return true;      // ignore the data

            SysExLength += 2;
            if (SysExLength > bufferSize) // SysEx is larger than the buffer
            {
#ifdef DEBUG
                Serial.println("SysEx is larger than buffer");
#endif
                startMessage(); // Discard message
                SysExLength = 0;
                return true;
            }
            if (!hasSpaceLeft(2)) // If there's no more free space in the buffer for two bytes
            {
                SysExLength -= 2;
                return false; // return, and don't increment USB buffer pointer
            }

            addToMessage(packet[1]); // add two data bytes to buffer
            addToMessage(SysExEnd);
            finishMessage(); // successfully added SysEx message to buffer, finish it
            SysExLength = 0;
        }
        else if (CIN == 0x70) // SysEx ends with following three bytes
        {
            if (SysExLength == 0) // If we haven't received a SysExStart
                return true;      // ignore the data

            SysExLength += 3;
            if (SysExLength > bufferSize) // SysEx is larger than the buffer
            {
#ifdef DEBUG
                Serial.println("SysEx is larger than buffer");
#endif
                startMessage(); // Discard message
                SysExLength = 0;
                return true;
            }
            if (!hasSpaceLeft(3)) // If there's no more free space in the buffer for three bytes
            {
                SysExLength -= 3;
                return false; // return, and don't increment USB buffer pointer
            }

            addToMessage(packet[1]); // add three data bytes to buffer
            addToMessage(packet[2]);
            addToMessage(SysExEnd);
            finishMessage(); // successfully added SysEx message to buffer, finish it
            SysExLength = 0;
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

        return true; // return, increment USB buffer pointer
    }

  public:
    bool refresh()
    {
#if defined(CORE_TEENSY) // If it's a Teensy board

        if (mod(readIndex - writeIndex, bufferSize) < 3)
            ; // return false;

        if (rx_packet == nullptr) // If there's no previous packet
        {
            if (!usb_configuration) // Check USB configuration
                return false;
            rx_packet = usb_rx(MIDI_RX_ENDPOINT); // Read a new packet from the USB buffer
            if (rx_packet == nullptr)             // If there's no new packet, return
                return false;
            if (rx_packet->len < 4) // If the lenght is less than 4, it's not a valid MIDI USB packet
            {
                usb_free(rx_packet); // Free the packet
                rx_packet = nullptr; // Read new packet on next refresh
                return true;
            }
        }

        size_t index = rx_packet->index;

        uint8_t *data = rx_packet->buf + index; // A pointer to this packet

        if (!parseUSBMIDIpacket(data)) // add the packet to the MIDI buffer
            return false;              // if it fails, return false, it means that the buffer is full, so parse the messages in the buffer first, don't throw away the USB packet just yet

        index += 4;
        if (index < rx_packet->len) // If the packet is longer than 4 bytes
        {
            rx_packet->index = index; // Change the index and read the next four bytes on next refresh
        }
        else // If the packet was only 4 bytes long
        {
            usb_free(rx_packet);                  // Free the packet
            rx_packet = usb_rx(MIDI_RX_ENDPOINT); // Read the next packet
        }

        return true;

#elif defined(USBCON) // If the main MCU has a USB connection but is not a Teensy
        midiEventPacket_t rx;
        rx = MidiUSB.read();
        if (rx.header != 0)
        { // TODO
            Serial.print("Received: ");
            Serial.print(rx.header, HEX);
            Serial.print("-");
            Serial.print(rx.byte1, HEX);
            Serial.print("-");
            Serial.print(rx.byte2, HEX);
            Serial.print("-");
            Serial.println(rx.byte3, HEX);
            return true;
        }
        return false;
#endif
    }
#if defined(CORE_TEENSY)
  private:
    usb_packet_t *rx_packet = nullptr;
#endif
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