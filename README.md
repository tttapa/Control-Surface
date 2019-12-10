[![Build Status](https://github.com/tttapa/Control-Surface/workflows/CI%20Tests/badge.svg)](https://github.com/tttapa/Control-Surface/actions)
[![Test Coverage](https://img.shields.io/endpoint?url=https://raw.githubusercontent.com/tttapa/Control-Surface-doc/master/docs/Coverage/shield.io.coverage.json)](https://tttapa.github.io/Control-Surface-doc/Coverage/index.html)
[![Build Status](https://travis-ci.org/tttapa/Control-Surface.svg?branch=master)](https://travis-ci.org/tttapa/Control-Surface)
[![GitHub](https://img.shields.io/github/stars/tttapa/Control-Surface?label=GitHub&logo=github)](https://github.com/tttapa/Control-Surface)

# Control Surface

An Arduino library for MIDI control surfaces (input and output).

## Overview

This library turns your Arduino-compatible board into a MIDI control surface.  

Just connect up some push buttons, potentiometers, LEDs ... and declare them in
your code.

Multiple different MIDI interfaces are supported: **MIDI over USB**, Serial MIDI 
(e.g. **5-pin DIN MIDI**), **Debug MIDI** (prints out the messages in a readable
format, and allows you to input text based messages), **MIDI over Bluetooth LE**
(experimental).

For MIDI output, you can use **push buttons**, toggle switches, 
**potentiometers**, faders, **rotary encoders**, etc.

All digital inputs are **debounced**, and all analog inputs are filtered using 
**digital filters and hysteresis**. This results in high accuracy without noise,
without introducing latency.

These can be used to send MIDI notes, Control Changes, Pitch Bends, 
Program/Patch changes, etc.

For MIDI input, you can use **LEDs** to display the state of different settings, 
to display the audio level of each channel (**VU meters**), the positions of knobs 
(V-Pot **LED rings**), etc.  
You can also add an **OLED display** (e.g. SSD1306) to display a nice overview 
of which channels are set to mute or solo, display the time cursor, VU meters,
etc. Bitmaps with play, mute, solo, record buttons, and others are included.  

A large portion of the **Mackie Control Universal** protocol is implemented.

All controls can be arranged in **banks**: e.g. if you have only 4 physical 
volume faders, you can add them to a bank, and then you can control 8 channels 
(or more) by changing the bank setting.

Apart from banks and bank selectors, you can also add **transposers** to change 
the key of your notes, for example.

In order to save some IO pins, the library natively supports **Shift Registers** 
(e.g. 74HC595) and **multiplexers** (e.g. 74HC4051 or 74HC4067).

If you are using a Teensy 3.x, you can use it as a **USB audio interface**. Just
add an I²S DAC (e.g. PCM5102), and 5 lines of code, and you can start playing 
audio through your Teensy.  
You can also add volume controls and VU meters for these audio connections.

Thanks to the structure of the library, you can easily add your own MIDI or 
display elements, using some minimal, high level code. All low level stuff is
completely **reusable** (e.g. all MIDI operations, debouncing switches, 
filtering analog inputs, and so on).

## Example usage

A complete sketch for a MIDI controller with a potentiometer that sends out MIDI
Control Change message can be written in just five lines of code:

```cpp
#include <Control_Surface.h>

USBMIDI_Interface midi;
CCPotentiometer pot = { A0, MIDI_CC::General_Purpose_Controller_1 };

void setup() { Control_Surface.begin(); }
void loop() { Control_Surface.loop(); }
```

Larger MIDI controllers can implemented very easily as well, with clean and easy
to modify code.  
The following sketch is for 8 potentiometers (connected using an analog
multiplexer) that send out MIDI Control Change messages over USB.

```cpp
#include <Control_Surface.h>  // Include the library
 
USBMIDI_Interface midi;  // Instantiate a MIDI Interface to use
 
// Instantiate an analog multiplexer
CD74HC4051 mux = {
  A0,       // Analog input pin
  {3, 4, 5} // Address pins S0, S1, S2
};
 
// Create an array of potentiometers that send out MIDI Control Change messages 
// when you turn the potentiometers connected to the 8 input pins of the mux.
CCPotentiometer volumePotentiometers[] = {
  { mux.pin(0), { MIDI_CC::Channel_Volume, CHANNEL_1 } },
  { mux.pin(1), { MIDI_CC::Channel_Volume, CHANNEL_2 } },
  { mux.pin(2), { MIDI_CC::Channel_Volume, CHANNEL_3 } },
  { mux.pin(3), { MIDI_CC::Channel_Volume, CHANNEL_4 } },
  { mux.pin(4), { MIDI_CC::Channel_Volume, CHANNEL_5 } },
  { mux.pin(5), { MIDI_CC::Channel_Volume, CHANNEL_6 } },
  { mux.pin(6), { MIDI_CC::Channel_Volume, CHANNEL_7 } },
  { mux.pin(7), { MIDI_CC::Channel_Volume, CHANNEL_8 } },
};
 
void setup() {
  Control_Surface.begin();  // Initialize the Control Surface
}

void loop() {
  Control_Surface.loop();  // Update the Control Surface
}
```

Control Surface supports many types of MIDI inputs. 
For example, an LED that turns on when a MIDI Note On message for middle C is
received:
```cpp
#include <Control_Surface.h>

USBMIDI_Interface midi;
using namespace MIDI_Notes;
NoteValueLED led = { LED_BUILTIN, note(C, 4) };

void setup() { Control_Surface.begin(); }
void loop() { Control_Surface.loop(); }
```
## Getting Started

See the [**Getting Started**](https://tttapa.github.io/Control-Surface-doc/Doxygen/d5/d7d/md_pages_Getting-Started.html)
page to get started using the library.

## Documentation

The automatically generated Doxygen documentation for this library can be found 
here:  
[**Documentation**](https://tttapa.github.io/Control-Surface-doc/Doxygen/index.html)  
Test coverage information can be found here:  
[**Code Coverage**](https://tttapa.github.io/Control-Surface-doc/Coverage/index.html)  
Arduino examples can be found here:  
[**Examples**](https://tttapa.github.io/Control-Surface-doc/Doxygen/examples.html)

Have a look at the [**modules**](https://tttapa.github.io/Control-Surface-doc/Doxygen/modules.html)
for an overview of the features of the library.

## The Control Surface library vs. The MIDI Controller library

You might already have found my other Arduino MIDI library, [MIDI Controller](https://github.com/tttapa/MIDI_Controller), 
and are wondering which one you should use for your project.

First, some background:  
I first started working on the MIDI Controller library way back in 2015, and it
evolved a lot early on. The library seemed to be pretty popular, and it worked
pretty well, so I couldn't just push breaking changes every couple of months.  
Many people requested support for MIDI input, and I liked experimenting with it
as well. The main problem was that the overall architecture of the library 
needed a complete overhaul in order to add MIDI input support. Since I didn't 
know if the MIDI input was going to work out, and I didn't want to break 
compatibility with older versions of the library, I decided to fork it: Control
Surface was born.  
At the moment, I consider the MIDI Controller library "complete". I won't be 
adding any groundbreaking new features, but I will still be fixing bugs and 
providing support.  
Control Surface, on the other hand, is where the active development takes place.

The main difference between the two libraries is that Control Surface has much
more features. MIDI Controller has everything you need for a working MIDI 
controller with potentiometers, push buttons, rotary encoders, etc., while 
Control Surface supports all of that, plus MIDI input, LEDs, VU meters, OLED 
displays, MIDI over Bluetooth, Audio over USB, etc.  
Another major difference is the documentation and tests. Control Surface tries
to provide better documentation using Doxygen, and it has many unit tests to 
make sure I don't introduce any bugs.

For a new project, I would recommend Control Surface, because I think it has 
some great features compared to MIDI Controller.  
The only caveat is that this library is still under development. Master should 
always be relatively stable, but I might change the API of some parts of the 
library for future releases if necessary.  
Another thing is that not everything is implemented yet, and many features are 
not yet fully documented. If you have a specific feature request that is not yet
fully implemented, feel free to open an issue, so I know where to focus on first.

## Work in progress

- Adding support for motorized faders
- Cleaning up the display code
- Cleaning up the MIDI over Bluetooth LE code
- Adding more tests (currently at over 320 unit tests)
- Adding more examples and adding comments to existing examples
- Finishing the documentation

## Supported boards

For each commit, the continuous integration tests compile the examples for the
following boards:

- Arduino UNO
- Arduino Leonardo
- Teensy 3.2
- Arduino Due
- Arduino Nano 33 IoT
- ESP8266
- ESP32

This covers a very large part of the Arduino platform, and similar boards will
also work (e.g. Arduino Nano, Arduino Mega, etc.).

If you have a board that's not supported, please 
[open an issue](https://github.com/tttapa/Control-Surface/issues/new)
and let me know!

Note that MIDI over USB and MIDI over Bluetooth are not supported on all boards.  
For MIDI over USB support, check out the [**MIDI over USB**](https://tttapa.github.io/Control-Surface-doc/Doxygen/d8/d4a/md_pages_MIDI-over-USB.html)
documentation page. As a general rule of thumb, if your board is supported by 
the [MIDIUSB library](https://github.com/arduino-libraries/MIDIUSB) or if it's
a Teensy, MIDI over USB should be supported.  
MIDI over BLE is currently only supported on ESP32.

## Information for developers

Information for people that would like to help improve the Control Surface 
library can be found here: 
<https://tttapa.github.io/Pages/Arduino/Control-Surface/Developers/index.html>  
It covers installation instructions for developers, instructions for running the
tests and generating documentation, a style guide, etc.

## Recent Breaking Changes

 - bca6e11b2b3e02df5f600f65c81676708a81155b  
   The color mapper for `NoteRangeFastLED` and the like now takes a second 
   parameter that represents the index of the LED within the LED strip.
 - 3c01c7d5eb60e59720540d5a77095468e6984a58  
   The **maximum supported ADC resolution is now used by default** (e.g. 13 bits
   on Teensy 3.x, 12 bits on ESP32).  
   This increases the accuracy of analog inputs and controls for the Control 
   Surface library, but could cause problems if your code uses other libraries
   that expect the resolution to be 10 bits.  
   You can change the default resolution to 10 bits in 
   [`src/AH/Settings/Settings.hpp`](https://tttapa.github.io/Control-Surface-doc/Doxygen/dc/d69/namespaceAH.html#a4f2ec536d7413c6969f44d63ba14ce55)
   if you have to.
 - 31edaa6b76477fdf152c19fd34f7e4e8506561e6  
   The **mapping function** is now applied before applying hysteresis.  
   This means that the input and output values of the function should be 
   16 - `ANALOG_FILTER_SHIFT_FACTOR` bits wide instead of 7. By default this is
   **14 bits**.  
   The signature of the mapping function is now `analog_t f(analog_t raw)`, 
   where the return value and raw are both numbers in [0, 16383].
