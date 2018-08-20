#if defined (ARDUINO) && defined (TEENSYDUINO)

#include "usb_names.h"

#define MIDI_NAME  {'C','o','n','t','r','o','l',' ','S','u','r','f','a','c','e'}
#define MIDI_NAME_LEN  15

struct usb_string_descriptor_struct usb_string_product_name = {
        2 + MIDI_NAME_LEN * 2,
        3,
        MIDI_NAME
};

#endif