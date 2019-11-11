[![Build Status](https://travis-ci.org/tttapa/Arduino-Helpers.svg?branch=master)](https://travis-ci.org/tttapa/Arduino-Helpers)

# Arduino Helpers

## Features

- **Containers**: Arrays, Bit Arrays, Linked Lists, Smart Pointers.
- **Extended Input/Output**: Use shift registers and multiplexers as if they
  were normal Arduino IO pins.
- **Hardware Utilities**: Debounced buttons, long press buttons, button 
  matrices, filtered analog inputs, LED ranges, MAX7219 displays and LED 
  matrices.
- **Filters**: Exponential Moving Average filters, Hysteresis.
- **Math functions**: `min` and `max` functions instead of macros, functions
  for uniformly increasing the bit depth of signals.
- **Timing Utilities**: Blink Without Delay-style timers.
- **C++ STL Implementation** (partial): Some useful parts of the C++ Standard
  Template Library, adapted to work on AVR Arduinos. Includes the 
  `<type_traits>`, `<algorithm>`, `<utility>` and `<limits>` headers, among 
  others.

These utilities were originally part of the
[Control Surface library](https://github.com/tttapa/Control-Surface),
and were split off to be used as a stand-alone library, or as a template for 
other libraries.

## Documentation

The automatically generated Doxygen documentation for this library can be found 
[here](https://tttapa.github.io/Arduino-Helpers/Doxygen/index.html).  
Test coverage information can be found 
[here](https://tttapa.github.io/Arduino-Helpers/Coverage/index.html).
