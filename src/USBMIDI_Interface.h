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

    void parseUSBMIDIpacket(uint8_t *packet)
    {
        Serial.printf("\t\t\t\t\t\tMIDIUSB packet:\t%02X %02X %02X %02X\r\n", packet[0], packet[1], packet[2], packet[3]);

        uint8_t CIN = (packet[0] & 0b1111) << 4; // MIDI USB cable index number

        if (CIN >= NOTE_OFF && CIN <= PITCH_BEND) // 2- or 3-byte MIDI event
        {
            uint8_t header = packet[1] & 0xFF;
            uint8_t type = header & 0xF0;
            if (CIN != type)
                return;
            ringbuffer[writeIndex] = header;
            incrementWriteIndex(1);
            ringbuffer[writeIndex] = packet[2];
            incrementWriteIndex(1);
            if (CIN != PROGRAM_CHANGE && CIN != CHANNEL_PRESSURE) // 3-byte MIDI event
            {
                ringbuffer[writeIndex] = packet[3];
                incrementWriteIndex(1);
            }
            availableMIDIevents++;
            return;
        }
        else if (CIN == 0x40) // SysEx starts or continues
        {
        }
    }

  public:
    bool refresh()
    {
#if defined(CORE_TEENSY)          // If it's a Teensy board
        if (rx_packet == nullptr) // If there's no previous packet
        {
            if (!usb_configuration) // Check USB configuration
                return false;
            rx_packet = usb_rx(MIDI_RX_ENDPOINT); // Read a new packet from the USB buffer
            if (rx_packet == nullptr)             // If there's no new packet, return
                return false;
            if (rx_packet->len < 4) // If the lenght is zero
            {
                usb_free(rx_packet); // Free the packet
                rx_packet = nullptr; // Read new packet on next refresh
                return true;
            }
        }

        size_t index = rx_packet->index;

        uint8_t *data = rx_packet->buf + index; // A pointer to this packet

        parseUSBMIDIpacket(data);

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