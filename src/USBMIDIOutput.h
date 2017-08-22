#ifndef USBMIDIOUTPUT_H_
#define USBMIDIOUTPUT_H_

#include "MIDIOutput.h"

#if defined(USBCON) && !defined(CORE_TEENSY) // If the main MCU has a USB connection but is not a Teensy
#include "MIDIUSB.h"
#endif

#if defined(CORE_TEENSY) && !(defined(USB_MIDI_AUDIO_SERIAL) || defined(USB_MIDI) || defined(USB_MIDI_SERIAL))
#error Please select a MIDI option in the 'Tools > USB Type' menu.
#endif

#if defined(USBCON) || defined(CORE_TEENSY) // If the main MCU has a USB connection (or is a Teensy)
class USBMIDIOutput : public MIDIOutput
{
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
};

#else // If the main MCU doesn't have a USB connection

#include "SerialMIDIOutput.h"

class USBMIDIOutput : public USBSerialMIDIOutput
{
  public:
    USBMIDIOutput() : USBSerialMIDIOutput(MIDI_BAUD){};
};

#endif

#endif // USBMIDIOUTPUT_H_