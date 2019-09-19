# Control Surface

An Arduino library for MIDI control surfaces (input and output).

## Under Construction

The library is not yet finished, and some parts of the API could change in the future.  
However, I believe that this is a useful library, even if not everything is 
implemented yet.  
If you'd like to help me with the development, please open an issue if you have requests 
or if you encounter any problems.

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

## Recent Breaking Changes

 - 31edaa6b76477fdf152c19fd34f7e4e8506561e6  
   The mapping function is now applied before applying hysteresis. This means 
   that the input and output values of the function should be 10 bits wide 
   instead of 7. The signature of the mapping function is now 
   `analog_t f(analog_t raw)`, where the return value and raw are both numbers
   in [0, 1023].

## Work in progress

- Adding support for motorized faders
- Cleaning up the display code
- Cleaning up the MIDI over Bluetooth LE code
- Finish support for MIDI over USB cable numbers
- Adding more tests (currently at over 170 unit tests)
- Adding more examples and adding comments to existing examples
- Finishing the documentation

## Getting Started

See the [Getting Started](https://tttapa.github.io/Control-Surface/Doc/Doxygen/dd/dcc/md_Getting-Started.html)
document to get started using the library.

## Documentation

The automatically generated Doxygen documentation for this library can be found 
[here](https://tttapa.github.io/Control-Surface/Doc/Doxygen/index.html).

## Information for developers

Information for people that would like to help improve the Control Surface library can be found here: <https://tttapa.github.io/Pages/Arduino/Control-Surface/Developers/index.html>  
It covers installation instructions for developers, instructions for running the
tests and generating documentation, a style guide, etc.

## Known problems

When compiling without LTO (Link Time Optimization) and -Os (optimize for size),
GCC will produce a linker error (`Undefined reference to ...`).  
This is normal, and it helps me to find other errors. I will eventually fix it
when I release a stable version of the library.  

For now, just enable LTO, or choose a different optimization option.
