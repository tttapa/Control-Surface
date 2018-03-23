#ifndef USBMIDI_INTERFACE_H_
#define USBMIDI_INTERFACE_H_

#include "MIDI_Interface.h"
#include "../MIDI_Parsers/USBMIDI_Parser.h"
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
    USBMIDI_Interface() : MIDI_Interface(parser)
    {
        ;
    }

  protected:
    USBMIDI_Parser parser;
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
    MIDI_read_t read()
    {
#if defined(CORE_TEENSY) // If it's a Teensy board
        while (1)
        {
#ifdef DEBUG
            // DEBUG_OUT << "while (1) USB" << endl;
#endif
            if (rx_packet == nullptr) // If there's no previous packet
            {
                if (!usb_configuration) // Check USB configuration
                    return NO_MESSAGE;
                rx_packet = usb_rx(MIDI_RX_ENDPOINT); // Read a new packet from the USB buffer
                if (rx_packet == nullptr) {            // If there's no new packet, return
#ifdef DEBUG
                    // DEBUG_OUT << "No USB packets" << endl;
#endif
                    return NO_MESSAGE;
                }
                if (rx_packet->len < 4) // If the lenght is less than 4, it's not a valid MIDI USB packet
                {
                    usb_free(rx_packet); // Free the packet
                    rx_packet = nullptr; // Read new packet next time around
#ifdef DEBUG
                    DEBUG_OUT << "rx_packet->len < 4" << endl;
#endif
                    return NO_MESSAGE;
                }
            }

            size_t index = rx_packet->index;

            uint8_t *data = rx_packet->buf + index; // A pointer to this packet

#ifdef DEBUG
            DEBUG_OUT << "Parsing ..." << endl;
#endif

            MIDI_read_t parseResult = parser.parse(data);

#ifdef DEBUG
            DEBUG_OUT << "parseResult = " << parseResult << endl;
#endif

            index += 4;
            if (index < rx_packet->len) // If the packet is longer than 4 bytes
            {
                rx_packet->index = index; // Change the index and read the next four bytes on next read
            }
            else // If the packet was only 4 bytes long
            {
                usb_free(rx_packet);                  // Free the packet
                rx_packet = usb_rx(MIDI_RX_ENDPOINT); // Read the next packet
#ifdef DEBUG
                DEBUG_OUT << "Free USB packet" << endl;
#endif
            }
            if (parseResult != NO_MESSAGE)
                return parseResult;
#ifdef DEBUG
            DEBUG_OUT << "loop USB MIDI" << endl;
#endif
        }

#elif defined(USBCON) // If the main MCU has a USB connection but is not a Teensy
        while (1)
        {
            rx_packet = *(uint32_t *)&MidiUSB.read();
            if (rx_packet == 0)
                return NO_MESSAGE;
            MIDI_read_t parseResult = parser.parse((uint8_t *)&rx_packet);
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