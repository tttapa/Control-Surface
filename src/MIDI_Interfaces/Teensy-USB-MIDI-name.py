midi_name = 'Control Surface'
midi_port_names = [
    'Control Surface',
    'Control Surface XT 1',
    'Control Surface XT 2',
    'Control Surface XT 3',
    'Control Surface XT 4',
    'Control Surface XT 5',
    'Control Surface XT 6',
    'Control Surface XT 7',
    'Control Surface XT 8',
    'Control Surface XT 9',
    'Control Surface XT 10',
    'Control Surface XT 11',
    'Control Surface XT 12',
    'Control Surface XT 13',
    'Control Surface XT 14',
    'Control Surface XT 15',
]

filename = 'Teensy-USB-MIDI-name.c'

import os
import sys

pythonfilename = os.path.basename(__file__)


def split_string_chars(string: str) -> str:
    return ', '.join(map(lambda c: '\'' + c + '\'', string))


warning = """
// =============== AUTOMATICALLY GENERATED FILE: DO NOT EDIT! =============== //
//                                                                            //
// Edit {filename} instead. {sp} //
""".format(filename=pythonfilename, sp=' ' * (59 - len(pythonfilename)))

output = warning + """
#if defined(ARDUINO) && defined(TEENSYDUINO) && !defined(__AVR_AT90USB1286__)
#include "usb_names.h"
"""

output += """
/// USB name: "{name}"
struct usb_string_descriptor_struct usb_string_product_name = {{
        2 + {len} * 2,
        3,
        {{{chars}}}
}};
""".format(name=midi_name,
           len=len(midi_name),
           chars=split_string_chars(midi_name))

i = 0
for port_name in midi_port_names:
    i += 1
    output += """
/// USB MIDI Port {index} name: "{name}"
struct usb_string_descriptor_struct usb_string_midi_port{index} = {{
        2 + {len} * 2,
        3,
        {{{chars}}}
}};
""".format(index=i,
           name=port_name,
           len=len(port_name),
           chars=split_string_chars(port_name))

output += """
#endif
"""

with open(os.path.join(sys.path[0], filename), 'w') as outputfile:
    outputfile.write(output)
