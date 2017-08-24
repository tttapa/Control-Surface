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
        strcat(stringToPrint, "Constructor USBMIDI_Interface\r\n");
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

  public:
    void refresh()
    {
#if defined(CORE_TEENSY)          // If it's a Teensy board
        if (rx_packet == nullptr) // If there's no previous packet
        {
            if (!usb_configuration) // Check USB configuration
                return;
            rx_packet = usb_rx(MIDI_RX_ENDPOINT); // Read a new packet from the USB buffer
            if (rx_packet == nullptr)             // If there's no new packet, return
                return;
            if (rx_packet->len == 0) // If the lenght is zero
            {
                usb_free(rx_packet); // Free the packet
                rx_packet = nullptr; // Read new packet on next refresh
                return;
            }
        }

        size_t index = rx_packet->index;

        uint32_t n = *(uint32_t *)(rx_packet->buf + index); // The 4-byte MIDI USB packet
        uint8_t *address = rx_packet->buf + index;          // A pointer to this packet

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

        uint8_t type1 = (n & 0b1111) << 4;     // MIDI message type from the first byte
        uint8_t type2 = (n >> 8) & 0b11110000; // MIDI message type from the second byte

        MIDI_event midievent;
        midievent.header = (n >> 8) & 0xFF;
        midievent.data1 = (n >> 16) & 0xFF;
        midievent.data2 = (n >> 24) & 0xFF;

        if (type1 != type2) // Both message types should match if it's a valid MIDI message
            return;

        if (!(type1 == NOTE_ON || type1 == NOTE_OFF || type1 == CC || type1 == PITCH_BEND)) // Only save Note events, Control Change and Pitch bends
            return;

        memcpy(&ringbuffer[writeIndex], address + 1, 3);               // Copy the 3-byte MIDI message to the ring buffer
        // ringbuffer[writeIndex] = midievent;
        writeIndex = writeIndex < bufferSize - 1 ? writeIndex + 1 : 0; // Increment the ring buffer write index


#elif defined(USBCON) // If the main MCU has a USB connection but is not a Teensy
        midiEventPacket_t rx;
        do
        {
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
            }
        } while (rx.header != 0);
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