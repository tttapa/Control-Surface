#!/usr/bin/env python3

import re
import os
from os.path import join, realpath, basename, splitext
from pathlib import Path

dirnm = os.path.dirname(os.path.realpath(__file__))
doxydir = join(dirnm, "..")
# exclude = re.compile(r'(examples/Arduino-Helpers/)|(examples/test/)')
exclude = re.compile(r"examples/test/")

template = """\
/**
 * @example   "{pathname}"
 * 
 * {title}
 * {underline}
 *{docstr}*/

"""

groups = {
    "Getting started": [
        ("1.First-Output", "Multiplexed potentiometers that send MIDI Control Change messages"),
        ("2.First-Input", "LEDs driven by a shift register that turn on/off when a MIDI Note On/Off message is received"),
    ],
    "MIDI interfaces": {
        "Interfaces, transports and routing": [
            ("MIDI_Pipes-Routing", "Setting up automatic routes for MIDI messages between multiple MIDI interfaces, using @ref MIDI_Pipe%s"),
            ("MIDI_Pipes-Filter", "More complicated MIDI routing rules that also filter and modify MIDI messages that travel through a MIDI_Pipe"),
            ("Serial-Interface", "Sending and receiving MIDI messages over a serial port (e.g. for 5-pin DIN MIDI)"),
            ("BLEMIDI-Adapter", "Sending and receiving MIDI messages over Bluetooth Low Energy (LE), and relaying them over Serial"),
            ("USBMIDI-Adapter", "Sending and receiving MIDI messages over USB, and relaying them over Serial"),
            ("USBHostMIDI_Interface-Passthrough", "Sending and receiving MIDI messages to and from USB MIDI devices connected to a Teensy's USB Host port, and relaying them over the USB Device port"),
            ("AppleMIDI", "Sending and receiving MIDI messages over the network (WiFi/Ethernet) using RTP-MIDI"),
            ("Debug-MIDI-Interface", "Printing and reading MIDI messages to/from the Serial monitor for debugging purposes"),
            ("MIDI-Monitor", "Turns the Arduino into a MIDI monitor by printing all messages it receives to the Serial monitor, and sending messages typed into the Serial monitor over MIDI"),
            ("MIDI-Monitor-OLED", "Prints all messages that are sent and received to a small OLED display"),
            ("Dual-MIDI-Interface", "Using multiple MIDI interfaces at the same time"),
        ],
        "Sending and receiving MIDI messages": [
            ("MIDI-Input", "Handling incoming MIDI messages using the @ref MIDI_Callbacks class"),
            ("MIDI-Input-Fine-Grained", "Handling incoming MIDI messages using the @ref FineGrainedMIDI_Callbacks class"),
            ("MIDI-Input-Fine-Grained-All-Callbacks", "Same as the previous, handles all possible message types"),
            ("MIDI-Output", "Sends MIDI Note On/Off message every second"),
            ("Send-MIDI-Notes", "Sends MIDI Note On/Off messages when a push button is pressed/released"),
            ("Send-All-MIDI-Messages", "Demonstrates how to send all possible MIDI message types"),
            ("SysEx-Send-Receive", "Sends and receives MIDI System Exclusive messages"),
        ]
    },
    "MIDI output elements": {
        "Potentiometers & Faders": [
            ("Control-Change-Potentiometer", "A single analog input from a potentiometer that sends MIDI Control Change messages"),
            ("Multiple-Control-Change-Potentiometers", "Multiple analog inputs"),
            ("CCPotentiometer-Map", "Applying a mapping function to the analog input to account for nonlinearities or calibrate the minimum and maximum values"),
            ("Pitch-Bend-Potentiometer", "A single analog input from a potentiometer that sends MIDI Pitch Bend messages (higher resolution than Control Change)"),
        ],
        "Buttons & Switches": [
            ("NoteButton", "A single digital input from a momentary push button that sends MIDI Note On/Off messages"),
            ("CCButton", "A single digital input from a momentary push button that sends MIDI Control Change messages"),
            ("NoteButtonMatrix", "A scanning keyboard matrix for a grid of momentary push buttons that send MIDI Note On/Off messages"),
            ("CCIncrementDecrementButtons", "Two momentary push buttons that increment/decrement a value by sending relative MIDI Control Change messages"),
        ],
        "Rotary Encoders": [
            ("RotaryEncoder", "A quadrature encoder that sends position changes using relative MIDI Control Change messages"),
            ("AbsoluteRotaryEncoder", "A quadrature encoder that sends its absolute position using MIDI Control Change messages"),
            ("MCP23017-RotaryEncoder-No-Interrupts", "Multiple quadrature encoders connected through a MCP23017 port expander that send position changes using relative MIDI Control Change messages"),
            ("MCP23017-RotaryEncoder-Interrupts", "Same as previous, but interrupt-driven (Teensy only)"),
        ],
        "Program Changers": [
            ("PCButton", "A single digital input from a momentary push button that sends a MIDI Program Change message when pressed"),
            ("Program-Changer", "Multiple momentary push buttons (or any other selector) that sends MIDI Program Change messages"),
            ("ManyAddressesPCButton", "Multiple buttons that send MIDI Program Change messages, and the programs they send are arranged in multiple banks, allowing you to select many programs using a limited number of buttons")
        ],
        "Other": [
            ("MIDI-Controller-Finished-Example", "An example MIDI Controller that combines switches, potentiometers, rotary encoders and banks"),
        ],
    },
    "MIDI input elements": {
        "LEDs": [
            ("1.Note-LED", "An LED that turns on when a MIDI Note On message for a specific note is received, and off when a Note Off message is received"),
            ("2.Note-Range-LEDs", "Similar to the previous, but for multiple LEDs and a range of consecutive notes"),
            ("3.NoteLEDBar", "An LED bar graph that displays the value of received MIDI Control Change messages"),
            ("4.VULEDs", "An LED bar graph that displays a level meter based on Mackie Control Universal MIDI messages sent by a DAW"),
            ("5.Note-LED-PWM", "An LED that changes brightness depending on the MIDI velocity value received"),
            ("6.MAX7219-NoteLED", "Similar to @ref 1.Note-LED.ino, but using a MAX7219 8×8 LED driver"),
            ("9.Note-FastLED", "Similar to @ref 1.Note-LED.ino, but for addressable LEDs using the FastLED library"),
            ("10.Note-FastLED-ColorMapper", "Similar to previous, but customizing the colors and intensity based on the note number and the velocity"),
        ],
        "Other": [
            ("Pitch-Bend-Value", "Keeps track of the value of incoming MIDI Pitch Bend messages"),
        ],
    },
    "Banks & Selectors": [
        ("Bank", "Using banks to control the volume of 8 tracks using only 2 physical potentiometers"),
        ("Transposer", "Using a transposer to change the pitch of some NoteButton%s using two push buttons"),
        ("Custom-Selector-Callback", "Changing the color of an RGB LED depending on the currently selected bank"),
        ("Bankable-Smart-Control-Change-Potentiometer", "Using potentiometers in banks can be tricky because they might send messages while cycling between banks, this issue is addressed by the @ref Bankable::CCSmartPotentiometer"),
    ],
    "Hardware and Extended Input/Output": [
        ("1.FilteredAnalog", "Filtering an analog input using a digital low-pass filter and hysteresis"),
        ("1.FilteredAnalog-Advanced", "Similar to previous, with more advanced features and options"),
        ("2.Button", "Debounce a switch or push button"),
        ("3.MultiPurposeButton", "Detect long presses and double presses of a push button"),
        ("1.AnalogReadSerial", "Printing the values of all channels of a multiplexer to the serial monitor"),
        ("2.DigitalReadSerial", "Similar to previous, but for digital inputs"),
        ("3.FilteredAnalogReadSerial", "Similar to @ref 1.AnalogReadSerial.ino, but filters the analog inputs"),
        ("1.SPI-Blink", "Blink an LED that's driven by a shift register connected using SPI"),
        ("2.BitBang-Blink", "Similar to previous, but using ordinary GPIO pins instead of SPI"),
        ("2.RGB-LED-Chaser", "%Display some fancy patterns using a handful of RGB LEDs driven by shift registers"),
        ("1.MAX7219-Blink", "Blink an LED that's driven by a MAX7219 8×8 LED driver"),
        ("MCP23017", "Read input from a push button and turn on/off an LED using a MCP23017 I²C port expander"),
        ("Toggle-LEDs", "Read digital inputs from 8 push buttons connected through a multiplexer, and use them to toggle 8 LEDs on/off, using a shift register to drive them"),
        ("MCP23017-Encoders-No-Interrupts", "Read multiple quadrature encoders using a MCP23017 port expander"),
        ("MCP23017-Encoders-Interrupts", "Same as previous, but interrupt-driven (Teensy only)"),
        ("Pin-Change-Interrupt-Encoders", "Read multiple quadrature encoders using pin change interrupts instead of polling (AVR only)"),
        ("Timer-Interrupt-Encoders", "Use a timer interrupt to poll multiple quadrature encoders at a consistent rate (AVR only)"),
    ],
    "Displays": [
        ("MCU-OLED-SSD1306", "Demonstrates the use of DisplayElement%s to display information from the DAW on a small OLED display"),
        ("MCU-OLED-SSD1306-x2", "Similar to previous, with two OLED displays instead of one"),
    ],
    "Board-specific examples": {
        "Teensy": [
            ("1.Volume-Controlled-USB-DAC", "USB audio interface with a potentiometer to control the volume"),
            ("2.VU-Meter-OLED-USB-DAC", "USB audio interface with a VU meter displayed on a small OLED display"),
        ],
        "ESP32": [
            ("VU-Meter-Bridge", "Mackie Control Universal control surface with an OLED display for real-time level meters and a MIDI over BLE interface"),
        ]
    },
    "Extending the library": [
        ("Custom-USB-MIDI-Backend", "Adding support for alternative MIDI over USB libraries (host and device)"),
        ("Custom-MIDI-Sender", "Customize the type of messages sent by the provided MIDI output elements"),
        ("Custom-MIDI-Output-Element", "Adding your own MIDI output elements, recreating and then customizing the @ref NoteButton class"),
        ("Custom-MIDI-Output-Element-Bankable", "Same as previous, but for the @ref Bankable::NoteButton class"),
        ("Custom-MIDI-Input-Element", "Adding your own customizable MIDI input elements, recreating the @ref NoteLED class"),
        ("Custom-Note-LED-Input-Element-Callback", "Similar to previous, but with PWM output"),
        ("Custom-Note-LED-Input-Element-Callback-FastLED", "Similar to previous, but with support for FastLED addressable LED strips"),
    ],
    "Other examples": [
        ("Debug", "How use the debug macros for printing different kinds of debugging information"),
        ("BlinkWithoutDelay-Timer", "Non-blocking delay using the Timer class"),
        ("Mackie-Control-Universal-Reverse-Engineering", "Translates incoming MIDI messages to the corresponding Mackie Control Universal names and prints them to the Serial monitor"),
        ("MIDI-Input-Callback", "Uses the MIDI input callbacks of the Control_Surface_ class to intercept MIDI input"),
        ("Keyboard-Matrix-BCD", None),
        ("Bank-Button-Matrix", None),
        ("Encoder-Selector-Bank", None),
        ("Note-ManyAddresses-Transposer", None),
        ("One-Pot-Both-PB-and-CC", None),
        ("MIDI_controller-97", None),
        ("Transpose-Octave-NC-Button", None),
    ],
}


def compose_examples_list(examples, listed_examples: set):
    if isinstance(examples, dict):
        for values in examples.values():
            compose_examples_list(values, listed_examples)
    else:
        for example, _ in examples:
            listed_examples.add(example)


listed_examples = set()
compose_examples_list(groups, listed_examples)
print(listed_examples)


def stripQuotes(string):
    string = re.sub(r'^"', "", string)
    string = re.sub(r'"$', "", string)
    return string


with open(join(doxydir, "Doxyfile")) as doxy:
    doxy_content = doxy.read()
    m = re.search(r"EXAMPLE_PATH\s*=\s*(.+)", doxy_content)
    if m:
        exampledir = m.group(1).split('" "')[0]
        exampledir = realpath(join(doxydir, stripQuotes(exampledir)))
        print("Example directory =", exampledir)
    else:
        print("Error: couldn't find EXAMPLE_PATH in Doxyfile")
        exit(1)
    m = re.search(r"INPUT\s*=\s*([./a-zA-Z0-9_-]+)", doxy_content)
    if m:
        lastInclude = m.group(1).split(" ")[-1]
        outputfile = realpath(join(doxydir, stripQuotes(lastInclude)))
        print("Output file =", outputfile)
    else:
        print("Error: couldn't find INPUT in Doxyfile")
        exit(1)

output = ""

for root, dirs, files in os.walk(exampledir):
    if exclude is not None and re.search(exclude, root) is not None:
        print("Excluding", root)
        continue
    print("Scanning", root)
    for file in sorted(files):
        if file.endswith(".ino"):
            with open(join(root, file)) as example:
                example_content = example.read()
                s = example_content.split("/**", 1)
                if len(s) > 1:
                    print("\t\033[0;32mFound documentation for", file, "\033[0m")
                    docstr = s[1].split("*/", 1)[0]
                    # Add line break after brief
                    # docstr = re.sub(r'^\s*\*\s*$', r'\g<0><br>  ', docstr, 1,
                    #                 re.MULTILINE)
                    # pathname = str(Path(root).relative_to(exampledir) / file)
                    pathname = file
                    title = splitext(basename(file))[0]
                    underline = "=" * len(title)
                    output += template.format(
                        pathname=pathname,
                        title=title,
                        underline=underline,
                        docstr=docstr,
                    )
                    if title not in listed_examples:
                        groups["Other examples"].append((title, None))
                else:
                    print("\t\033[0;33mWarning: no documentation for", file, "\033[0m")
                    print('\t       → "' + str(Path(root) / file) + '"')


def add_page(title: str, groups, prefix="", level=0):
    output = ""
    if level > 0:
        prefix += "-" + title
        sanitized = re.sub("[^a-z0-9]", "-", prefix.lower())
        sanitized = re.sub("(-+)", "-", sanitized)
        sanitized = re.sub("-+$|^-+", "", sanitized)
        output += f" * {'#' * level} {title} {{#examples-{sanitized}}}\n"
    if isinstance(groups, dict):
        for title, subgroups in groups.items():
            output += add_page(title, subgroups, prefix, level + 1)
    else:
        for example, descr in groups:
            output += " * - @ref " + example + ".ino\n"
            if descr:
                output += " *   (" + descr + ")\n"
        output += " * \n"

    return output


output += "/**\n" " * @page examples Examples\n * \n * @tableofcontents\n * \n"
output += add_page(None, groups)
output += " */"

with open(outputfile, "w") as f:
    f.write(output)
