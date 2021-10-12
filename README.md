[![Build Status](https://github.com/tttapa/Control-Surface/workflows/CI%20Tests/badge.svg?branch=master)](https://github.com/tttapa/Control-Surface/actions?query=workflow%3A"CI+Tests")
[![Test Coverage](https://img.shields.io/endpoint?url=https://raw.githubusercontent.com/tttapa/Control-Surface-doc/master/docs/Coverage/shield.io.coverage.json)](https://tttapa.github.io/Control-Surface-doc/Coverage/index.html)
[![Examples](https://github.com/tttapa/Control-Surface/workflows/Examples/badge.svg?branch=master)](https://github.com/tttapa/Control-Surface/actions?query=workflow%3AExamples)
[![GitHub](https://img.shields.io/github/stars/tttapa/Control-Surface?label=GitHub&logo=github)](https://github.com/tttapa/Control-Surface)

# Control Surface

Control Surface is an Arduino library for building MIDI controllers and control
surfaces.
At its core is a
[general-purpose MIDI abstraction layer](https://tttapa.github.io/Control-Surface-doc/Doxygen/d3/df7/midi-tutorial.html)
with support for serial MIDI, MIDI over USB, MIDI over BLE, etc., which can be
useful for any MIDI-related project.  
Besides MIDI input/output, Control Surface also includes easy-to-use utilities
specifically for building MIDI controllers, supporting controls that send MIDI
─ like potentiometers, push buttons, rotary encoders, etc. ─
and controls that react to incoming MIDI ─ LEDs, displays, and so on.
They can also be combined into controls that use both MIDI input and output, 
such as motorized faders.

> Table of contents  
> <span class="mono">¶</span>&emsp;[Overview](#overview)  
> <span class="mono">¶</span>&emsp;[Example usage](#example-usage)  
> <span class="mono">¶</span>&emsp;[Getting started](#getting-started)  
> <span class="mono">¶</span>&emsp;[Documentation](#documentation)  
> <span class="mono">¶</span>&emsp;[Work in progress](#work-in-progress)  
> <span class="mono">¶</span>&emsp;[Supported boards](#supported-boards)  
> <span class="mono">¶</span>&emsp;[Information for developers](#information-for-developers)  
> <span class="mono">¶</span>&emsp;[Change log and updating](#change-log-and-updating)  

<a name="overview"></a>

## Overview

This library turns your Arduino-compatible board into a MIDI control surface.  
Just connect some push buttons, potentiometers, LEDs ... and declare them in
your code.

The following sections give a brief overview of the features of the library.

### MIDI Interfaces

 - **MIDI over USB**
 - **Serial MIDI** (e.g. 5-pin DIN MIDI)
 - **Debug MIDI** (prints out the messages in a readable format, and allows you
                   to input text based messages, like a MIDI monitor)
 - **MIDI over Bluetooth LE**
 - **AppleMIDI** over WiFi or Ethernet

<sub>→ [_MIDI Interfaces documentation_](https://tttapa.github.io/Control-Surface-doc/Doxygen/dc/df0/group__MIDIInterfaces.html)</sub>

### MIDI Control Output

 - **Push buttons** and **toggle switches**
 - **Potentiometers**, **faders** and other analog sensors
 - **Rotary encoders**
 - **Scanning keyboard matrices**

Digital inputs are **debounced**, and analog inputs are filtered using 
**digital filters and hysteresis**. This results in high accuracy without noise,
without introducing latency.

These MIDI control outputs can be used to send MIDI notes, Control Change,
Pitch Bend, Program/Patch change, etc.

<sub>→ [_MIDI Output Elements documentation_](https://tttapa.github.io/Control-Surface-doc/Doxygen/d7/dcd/group__MIDIOutputElements.html)</sub>

### MIDI Control Input

 - **LEDs** (e.g. to indicate whether a track is muted/armed/soloed)
 - **LED rings** (e.g. to indicate the position of a pan knob)
 - **LED strips** (using the [FastLED](https://github.com/FastLED/FastLED) 
                   library)
 - **VU meters**
 - **OLED displays**
 - **7-segment displays**

A large portion of the **Mackie Control Universal** (MCU) protocol is 
implemented.

<sub>→ [_MIDI Input Elements documentation_](https://tttapa.github.io/Control-Surface-doc/Doxygen/df/d8b/group__MIDIInputElements.html)</sub>

### Motorized faders

- **Motorized faders** are supported through the [tttapa/Control-Surface-Motor-Fader](https://github.com/tttapa/Control-Surface-Motor-Fader) repository.

<sub>→ [_Control Surface Motor Fader documentation_](https://tttapa.github.io/Pages/Arduino/Control-Theory/Motor-Fader/)</sub>

### Bank support

All controls can be arranged in **banks**: for example, if you have only 4 
physical faders, you can make them bankable, so they can be used to control 
the volume of many different tracks, by selecting the corresponding bank.

Selecting a bank can be done using push buttons, rotary encoders, etc.

Apart from banks and bank selectors, you can also add **transposers** to change 
the key of your notes, for example.

### Extended input/output

In order to save some IO pins, the library natively supports **multiplexers** 
(e.g. 74HC4051 or 74HC4067) to read many switches or potentiometers, 
**Shift Registers** (e.g. 74HC595) to drive many LEDs, **MAX7219 LED drivers**,
etc.

<sub>→ [_Extended IO documentation_](https://tttapa.github.io/Control-Surface-doc/Doxygen/db/dd3/group__AH__ExtIO.html)</sub>

### Audio

If you are using a Teensy 3.x or 4.x, you can use it as a 
**USB audio interface**. Just add an I²S DAC (e.g. PCM5102) and 5 lines of code,
and you can start playing audio through your Teensy, by combining Control 
Surface with the Teensy Audio library.  
You can also add volume controls and VU meters for these audio connections.

<sub>→ [_Teensy Audio documentation_](https://tttapa.github.io/Control-Surface-doc/Doxygen/d3/d5c/group__Audio.html)</sub>

### Modular and extensible

Thanks to the structure of the library, you can easily add your own MIDI or 
display elements, using some minimal, high level code. All low level stuff is
completely **reusable** (e.g. all MIDI operations, debouncing switches, 
filtering analog inputs, and so on).

<a name="example-usage"></a>

## Example usage

A complete sketch for a MIDI controller with a potentiometer that sends out MIDI
Control Change message can be written in just five lines of code:

```cpp
#include <Control_Surface.h>

USBMIDI_Interface midi;
CCPotentiometer pot { A0, MIDI_CC::General_Purpose_Controller_1 };

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
CD74HC4051 mux {
  A0,       // Analog input pin
  {3, 4, 5} // Address pins S0, S1, S2
};
 
// Create an array of CCPotentiometer objects that send out MIDI Control Change 
// messages when you turn the potentiometers connected to the 8 inputs of the mux.
CCPotentiometer volumePotentiometers[] {
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
NoteLED led { LED_BUILTIN, MIDI_Notes::C(4) };

void setup() { Control_Surface.begin(); }
void loop() { Control_Surface.loop(); }
```

<a name="getting-started"></a>

## Getting started

See the [**Getting Started**](https://tttapa.github.io/Control-Surface-doc/Doxygen/d5/d7d/md_pages_Getting-Started.html)
page to get started using the library.  
It'll also point you to the [**Installation Instructions**](https://tttapa.github.io/Control-Surface-doc/Doxygen/d8/da8/md_pages_Installation.html).

The [**MIDI tutorial**](https://tttapa.github.io/Control-Surface-doc/Doxygen/d3/df7/midi-tutorial.html)
might be useful if you want to use Control Surface as a regular MIDI library,
for sending and receiving MIDI messages.

<a name="documentation"></a>

## Documentation

The automatically generated Doxygen documentation for this library can be found 
here:  
[**Documentation**](https://tttapa.github.io/Control-Surface-doc/Doxygen/index.html)  
Test coverage information can be found here:  
[**Code Coverage**](https://tttapa.github.io/Control-Surface-doc/Coverage/index.html)  
Arduino examples can be found here:  
[**Examples**](https://tttapa.github.io/Control-Surface-doc/Doxygen/examples.html)

Have a look at the [**modules**](https://tttapa.github.io/Control-Surface-doc/Doxygen/modules.html)
for an overview of the features of the library, it's the best entry point for 
the documentation.

You can find an answer to some frequently asked questions on the 
[**FAQ**](https://tttapa.github.io/Control-Surface-doc/Doxygen/da/dc1/FAQ.html)
page.

<a name="work-in-progress"></a>

## Work in progress

- Adding more tests (currently at over 560 unit tests)
- Adding more examples and adding comments to existing examples
- Finishing the documentation

<a name="supported-boards"></a>

## Supported boards

For each commit, the continuous integration tests compile the examples for the
following boards:

- Arduino UNO
- Arduino Leonardo
- Teensy 3.2
- Arduino Due
- Arduino Nano Every
- Arduino Nano 33 IoT
- Arduino Nano 33 BLE
- ESP8266
- ESP32

This covers a very large part of the Arduino platform, and similar boards will
also work. For example, the Arduino Nano, Mega, Micro, Pro Micro, Teensy 2.0,
Teensy LC, Teensy 3.x, Teensy 4.x are all known to work.

If you have a board that's not supported, please 
[open an issue](https://github.com/tttapa/Control-Surface/issues/new)
and let me know!

Note that MIDI over USB and MIDI over Bluetooth are not supported on all boards.  
For MIDI over USB support, check out the [**MIDI over USB**](https://tttapa.github.io/Control-Surface-doc/Doxygen/d8/d4a/md_pages_MIDI-over-USB.html)
documentation page. As a general rule of thumb, if your board is supported by 
the [MIDIUSB library](https://github.com/arduino-libraries/MIDIUSB) or if it's
a Teensy, MIDI over USB should be supported.  
MIDI over BLE is currently only supported on ESP32.

<a name="information-for-developers"></a>

## Information for developers

Information for people that would like to help improve the Control Surface 
library can be found here: 
<https://tttapa.github.io/Pages/Arduino/Control-Surface/Developers/index.html>  
It covers installation instructions for developers, instructions for running the
tests and generating documentation, a style guide, etc.

<a name="change-log-and-updating"></a>

## Change log and updating

### 2.x

- ([c35f29c](https://github.com/tttapa/Control-Surface/commit/c35f29ced7f3e491467bd61c1c71013099c01091))  
   The SoftwareSerial MIDI interfaces are now in separate header files that have
   to be included explicitly if you want to use them. The headers in question 
   are [`SoftwareSerialMIDI_Interface.hpp`](https://tttapa.github.io/Control-Surface-doc/Doxygen/d3/df2/SoftwareSerialMIDI__Interface_8hpp.html)
   and [`SoftwareSerialDebugMIDI_Interface.hpp`](https://tttapa.github.io/Control-Surface-doc/Doxygen/de/d83/SoftwareSerialDebugMIDI__Interface_8hpp.html).
   This prevents redefinition errors of pin change interrupt vectors even if
   SoftwareSerial is not used.
- ([bf8fb66](https://github.com/tttapa/Control-Surface/commit/bf8fb661fa97fb6420584362bfcee7ce67939f59))  
   The abbreviated MIDI send functions (such as `sendCC()`) have been deprecated
   in favor of the full names (e.g. `sendControlChange()`). See the 
   [MIDI_Sender documentation](https://tttapa.github.io/Control-Surface-doc/Doxygen/d6/d72/classMIDI__Sender.html)
   for a full overview.
- ([cf32e7e](https://github.com/tttapa/Control-Surface/commit/cf32e7e72d81269e97f18ba51c42ba10715d1852))  
   `Control_Surface.setMIDIInputCallbacks()` now takes four arguments instead of
   three, because a System Common callback was added.
- ([b727931](https://github.com/tttapa/Control-Surface/commit/b727931a44cb1e262ac32901f1e307583da90624))  
   The MIDI note name for the note F has been changed from `F` to `F_` in order
   to avoid conflicts with the `F()` macro and its functional equivalent 
   introduced [here](https://github.com/arduino/ArduinoCore-mbed/blob/7a8d3ee46262d943f7cc1158f8bce06f61c3ddb2/cores/arduino/Arduino.h#L35-L41).  
   It is now recommended to use `MIDI_Notes::C(4)` instead of `note(C, 4)`.
- ([a81bd19](https://github.com/tttapa/Control-Surface/commit/a81bd1927298decc2ea3852fd2f00e8028c14b81))  
   Classes that make use of the SPI interface now require you to pass the `SPI`
   object as a constructor argument. This allows you to use `SPI1` or `SPI2`
   (if available for your hardware).
- ([c6e35b9](https://github.com/tttapa/Control-Surface/commit/c6e35b960ac15bc1b5cf5c588139d437ccd9cb68))  
   The `NoteBitmapDisplay` class has been renamed to `BitmapDisplay`.
- ([37b6901](https://github.com/tttapa/Control-Surface/commit/37b6901cbe56babc47c297a132381d53deaa45e8))  
   The `NoteValueLED` and `CCValueLED` classes (and similar) have been replaced
   by `NoteLED` and `CCLED` respectively.  
   The display elements `BitmapDisplay`, `VPotDisplay`, `VUDisplay` etc. are 
   now generic in the type of value that they display. In most cases, you should
   be able to update your sketch by adding `<>` after the type names, e.g. 
   `BitmapDisplay<>`, `VPotDisplay<>`, etc.
- ([1a21d13](https://github.com/tttapa/Control-Surface/commit/1a21d1344f57066fa1eda3819eaf89cbbc79c14e))  
   The line numbers of `LCDDisplay` are now one-based: `1` is the first line and
   `2` is the second line. This is more consistent with the track parameter and
   the API of the rest of the library. (Before, the first line was `0` and the
   second line was `1`.)
- ([40e3d7a](https://github.com/tttapa/Control-Surface/commit/40e3d7a4377b281e58dd7725f1bb8c6198855ce6))  
   Control Surface now comes with an Encoder library out of the box. You no 
   longer have to install or include `Encoder.h` in your sketches.

### 1.x

- ([8a3b1b3](https://github.com/tttapa/Control-Surface/commit/8a3b1b314cf5b4aedf3ad60cbbc492fbcbb25c73))  
   Before, `Control_Surface.MIDI()` was used to get the MIDI interface used by
   Control Surface. This method was removed, because you can now connect 
   multiple interfaces to Control Surface, using the 
   [MIDI Pipe routing system](https://tttapa.github.io/Control-Surface-doc/Doxygen/df/d72/classMIDI__Pipe.html#details).
   To send MIDI using Control Surface, you can now use 
   `Control_Surface.sendCC(...)` and the other similar methods directly.
- ([8a3b1b3](https://github.com/tttapa/Control-Surface/commit/8a3b1b314cf5b4aedf3ad60cbbc492fbcbb25c73))  
   For the same reason as the bullet above, the `MultiMIDI_Interface` was
   removed, and has been replaced by the
   [MIDI Pipe routing system](https://tttapa.github.io/Control-Surface-doc/Doxygen/df/d72/classMIDI__Pipe.html#details).
- ([bca6e11](https://github.com/tttapa/Control-Surface/commit/bca6e11b2b3e02df5f600f65c81676708a81155b))  
   The color mapper for `NoteRangeFastLED` and the like now takes a second 
   parameter that represents the index of the LED within the LED strip.
- ([3c01c7d](https://github.com/tttapa/Control-Surface/commit/3c01c7d5eb60e59720540d5a77095468e6984a58))  
   The **maximum supported ADC resolution is now used by default** (e.g. 13 bits
   on Teensy 3.x, 12 bits on ESP32).  
   This increases the accuracy of analog inputs and controls for the Control 
   Surface library, but could cause problems if your code uses other libraries
   that expect the resolution to be 10 bits.  
   You can change the default resolution to 10 bits in 
   [`src/AH/Settings/Settings.hpp`](https://tttapa.github.io/Control-Surface-doc/Doxygen/dc/d69/namespaceAH.html#a4f2ec536d7413c6969f44d63ba14ce55)
   if you have to.
- ([31edaa6](https://github.com/tttapa/Control-Surface/commit/31edaa6b76477fdf152c19fd34f7e4e8506561e6))  
   The **mapping function** is now applied before applying hysteresis.  
   This means that the input and output values of the function should be 
   16 - `ANALOG_FILTER_SHIFT_FACTOR` bits wide instead of 7. By default this is
   **14 bits**. You can get the maximum value in a portable way by using the
   `FilteredAnalog<>::getMaxRawValue()` function.  
   The signature of the mapping function is now `analog_t f(analog_t raw)`, 
   where the return value and raw are both numbers in [0, 16383] by default.
