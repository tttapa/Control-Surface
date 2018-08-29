# Control Surface

An Arduino library for MIDI control surfaces (input and output).

## UNDER CONSTRUCTION

Should not be considered stable. API is bound to change in the near future. 
Master might not even compile on some boards.

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

## Work in progress

- Adding support for motorized faders
- Cleaning up the display code
- Cleaning up the MIDI over Bluetooth LE code
- Adding more tests (currently at over 150 tests)
- Adding more examples and adding comments to existing examples
- Finishing the documentation

## Documentation

The automatically generated Doxygen documentation for this library can be found 
[here](https://tttapa.github.io/Control-Surface/Doc/Doxygen/index.html). 
(incomplete)

## Building and running the tests

For help compiling and running the tests, see [Tests.md](Tests.md).

## Known problems

When compiling without LTO (Link Time Optimization) and -Os (optimize for size),
GCC will produce a linker error (`Undefined reference to ...`).  
This is normal, and it helps me to find other errors. I will eventually fix it
when I release a stable version of the library.  

For now, just enable LTO, or choose a different optimization option.