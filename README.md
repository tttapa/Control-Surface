[![Build Status](https://github.com/tttapa/Arduino-Helpers/workflows/CI%20Tests/badge.svg#)](https://github.com/tttapa/Arduino-Helpers/actions)
[![Test Coverage](https://img.shields.io/endpoint?url=https://raw.githubusercontent.com/tttapa/Arduino-Helpers/gh-pages/Coverage/shield.io.coverage.json)](https://tttapa.github.io/Arduino-Helpers/Coverage/index.html)
[![GitHub](https://img.shields.io/github/stars/tttapa/Arduino-Helpers?label=GitHub&logo=github)](https://github.com/tttapa/Arduino-Helpers)

# Arduino Helpers

## Features

- **Containers**: Arrays, Bit Arrays, Linked Lists, Smart Pointers.
- **Extended Input/Output**: Use shift registers (e.g. 74HC595), multiplexers
  (e.g. 74HC4051, 74HC4067) and IO expanders as if they were normal Arduino IO
  pins.
- **Hardware Utilities**: Debounced buttons, long press buttons, double-click
  buttons, button matrices, filtered analog inputs, MCP23017 Rotary Encoders,
  LED ranges, MAX7219 displays and LED matrices.
- **Filters**: Exponential Moving Average filters, Hysteresis.
- **Math functions**: `min` and `max` functions instead of macros, functions
  for uniformly increasing the bit depth of signals, all C++ standard library 
  functions, vectors, quaternions, Euler angles.
- **Timing Utilities**: Blink Without Delay-style timers.
- **C++ STL Implementation** (partial): Some useful parts of the C++ Standard
  Template Library, adapted to work on AVR Arduinos. Includes the 
  `<type_traits>`, `<algorithm>`, `<numeric>`, `<new>`, `<array>`, `<bitset>`, 
  `<optional>`, `<cmath>`, `<complex>`, `<cstdlib>`, `<cstdint>`, `<tuple>`,
  `<memory>` (`std::unique_ptr`), `<initialize_list>`, `<utility>` and 
  `<limits>` headers, among others.

These utilities were originally part of the
[Control Surface library](https://github.com/tttapa/Control-Surface),
and were split off to be used as a stand-alone library, or as a template for 
other libraries.

Libraries that use the Arduino Helpers library include:
- [**Arduino Filters**](https://github.com/tttapa/Arduino-Filters):
  FIR, IIR filtering library
- [**Control Surface**](https://github.com/tttapa/Control-Surface):
  library for creating MIDI Control Surfaces, with knobs and push buttons, 
  LEDs, displays, etc.

## Documentation

The automatically generated Doxygen documentation for this library can be found 
here:  
[**Documentation**](https://tttapa.github.io/Arduino-Helpers/Doxygen/index.html)  
Test coverage information can be found here:  
[**Code Coverage**](https://tttapa.github.io/Arduino-Helpers/Coverage/index.html)  
Arduino examples can be found here:  
[**Examples**](https://tttapa.github.io/Arduino-Helpers/Doxygen/examples.html)

## Installation

Download the repository as a ZIP archive by going to the [home page of the 
repository](https://github.com/tttapa/Arduino-Helpers) and clicking
the green <kbd>Code</kbd> button in the top right, then choosing “Download ZIP”.  
Alternatively, click the following direct download link:
<https://github.com/tttapa/Arduino-Helpers/archive/refs/heads/master.zip>

Open the Arduino IDE, and go to the _Sketch &gt; Include Library &gt; Add .ZIP
Library_ menu.  
Then navigate to your downloads directory where you just downloaded the 
library.  
Select it, and click _Ok_.

## Supported boards

For each commit, the continuous integration tests compile the examples for the
following boards:

- Arduino UNO
- Arduino Mega
- Arduino Leonardo
- Teensy 3.2
- Teensy 4.1
- Arduino Due
- Arduino Nano 33 IoT
- Arduino Nano 33 BLE
- Arduino Nano Every
- ESP8266
- ESP32
- Raspberry Pi Pico

This covers a very large part of the Arduino platform, and similar boards will
also work (e.g. Arduino Nano, Arduino Mega, etc.).

If you have a board that's not supported, please 
[open an issue](https://github.com/tttapa/Arduino-Helpers/issues/new)
and let me know!

### Known limitations

The Arduino Due toolchain provided by Arduino is very old, and has some 
configuration problems. As a result, some math functions are not available.  
There is nothing I can do about it in this library, it's a bug in the Arduino 
Due Core.

## Recent Breaking Changes

### 2.0-alpha

 - a81bd1927298decc2ea3852fd2f00e8028c14b81  
   Classes that make use of the SPI interface now require you to pass the `SPI`
   object as a constructor argument. This allows you to use `SPI1` or `SPI2`
   (if available for your hardware).
