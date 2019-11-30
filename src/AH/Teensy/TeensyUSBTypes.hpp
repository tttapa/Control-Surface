#pragma once

#ifdef TEENSYDUINO

#if defined(USB_MIDI_SERIAL) || defined(USB_MIDI4_SERIAL) ||                   \
    defined(USB_MIDI16_SERIAL) || defined(USB_MIDI_AUDIO_SERIAL) ||            \
    defined(USB_MIDI16_AUDIO_SERIAL) || defined(USB_MIDI) ||                   \
    defined(USB_MIDI4) || defined(USB_MIDI16) || defined(USB_EVERYTHING)
#define TEENSY_MIDIUSB_ENABLED
#endif

#if defined(USB_MIDI_SERIAL) || defined(USB_MIDI4_SERIAL) ||                   \
    defined(USB_MIDI16_SERIAL) || defined(USB_MIDI_AUDIO_SERIAL) ||            \
    defined(USB_MIDI16_AUDIO_SERIAL) || defined(USB_SERIAL_HID) ||             \
    defined(USB_EVERYTHING) || defined(USB_SERIAL)
#define TEENSY_SERIALUSB_ENABLED
#endif

#if defined(USB_MIDI_AUDIO_SERIAL) || defined(USB_MIDI16_AUDIO_SERIAL) ||      \
    defined(USB_AUDIO) || defined(USB_EVERYTHING)
#define TEENSY_AUDIOUSB_ENABLED
#endif

#endif