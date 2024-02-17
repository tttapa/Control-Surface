# Getting Started {#md_pages_Getting-Started}

[TOC]

## Installation                                                  {#installation}

Instructions on how to install the library and its dependencies can be found 
on the page [Installation](@ref pages/Installation.md).

## First Output: Creating a Basic MIDI Controller Sketch         {#first-output}

### 1. Include the library

Include the library so that you have access to all the classes and functions it
provides.

```cpp
#include <Control_Surface.h>
```

### 2. Instantiate a MIDI Interface               {#first-output-midi-interface}

If you want to send out or receive MIDI messages, you have to define at least 
one MIDI interface. If you don't do that, you'll get an error when calling
`Control_Surface.begin()`.

There are many different MIDI interfaces to choose from:

- `USBMIDI_Interface`: On boards that support MIDI over USB natively, this will
  do exactly what you'd expect. You just have to plug it into your computer, and
  it shows up as a MIDI device. 
  On Arduinos without native USB capabilities (e.g. UNO or MEGA), you have to 
  use custom firmware for the ATmega16U2.
- `HardwareSerialMIDI_Interface`: This interface will send and receive MIDI 
  messages over a hardware UART. You can use it for MIDI over a 5-pin DIN 
  connector, for example.
- `HairlessMIDI_Interface`: If you have an Arduino without native USB support, 
  an easy way to get the MIDI messages from your Arduino to your computer is 
  the [Hairless MIDI<->Serial Bridge](https://projectgus.github.io/hairless-midiserial/).
  In that case, you can use this MIDI interface. The default baud rate is 115200
  symbols per second.
- `BluetoothMIDI_Interface`: If you have an ESP32, you can send and receive MIDI
  messages over Bluetooth Low Energy. This interface is still very much
  experimental, but it's pretty cool. If you know more about the MIDI BLE
  protocol, feel free to suggest some improvements.
- `USBDebugMIDI_Interface`: Debugging MIDI Controllers can be very cumbersome. 
  There are MIDI monitors available, but you have to reconnect every time you 
  upload a new sketch, and sending MIDI to the Arduino is not always easy.  
  This interface is designed to help you with that. It prints outgoing MIDI
  messages to the Serial port in a readable format, and it allows you to enter 
  hexadecimal MIDI messages (as text) in the Serial monitor 
  (e.g. `90 3C 7F` to turn on middle C).

A complete overview of the available MIDI interfaces can be found in the 
@ref MIDIInterfaces module.

For now, we'll use the `USBMIDI_Interface` because it's probably the one you'll
use in your final program. Do keep in mind that not all boards support MIDI over
USB natively, for more details, see the @ref md_pages_MIDI-over-USB page.

You can give the interface any name you want. I'll be very original and
choose `midi`. It doesn't matter, and you don't need to use it afterwards, 
just defining the interface is enough, the Control Surface library will 
automatically detect and use it.

```cpp
USBMIDI_Interface midi;
```

> **Note**: some interfaces require additional parameters, for example, 
> the `USBDebugMIDI_Interface` needs to know the baud rate.  
> In that case, you can instantiate it as follows:  
> `USBDebugMIDI_Interface midi {115200};`

For a more detailed overview of MIDI interfaces and using them to send and 
receive MIDI message, have a look at the @ref midi-tutorial.

### 3. Add Extended Input/Output elements (optional)       {#first-output-extio}

If your MIDI Controller requires many in- or outputs, you'll run out of IO pins
really quickly. A solution is to use multiplexers or shift registers.  
The Control Surface Library supports both of these options, and makes it easy
to support other types of IO expanders in the future.

An overview of Extended Input/Output elements can be found in the @ref AH_ExtIO
module.

In this example, we'll use an 8-channel 74HC4051 analog multiplexer. This
allows us to read eight analog inputs using just one analog pin on the Arduino,
at the cost of only three digital output pins.

Each of the eight analog inputs of the multiplexer can be connected to the wiper
of one potentiometer.

We'll connect the three address lines of the multiplexer (`S0`, `S1` and `S2`)
to digital pins `3`, `4` and `5`. The output of the multiplexer goes to 
analog pin `A0`. Connect the enable pin (`Ē`) to ground.

```cpp
CD74HC4051 mux { A0, {3, 4, 5} };
```

### 4. Add MIDI Control Elements

Now, we can specify the objects that read the input of the potentiometers and
send out MIDI events accordingly.

Again, I'll refer to the overview of @ref MIDIOutputElements.

Let's define a single potentiometer on pin `A1` that sends out MIDI Control 
Change events.  
In the @ref CCPotentiometer::CCPotentiometer "documentation",
you'll find that the first argument for the `CCPotentiometer` constructor is the 
analog pin number, and the second is the MIDI address.  
The MIDI address is a structure that consists of an address number, 
the MIDI channel, and the cable number.  
In this case, the address number is the controller number, which is a number
from 0 to 119. The MIDI channel is a channel from `Channel_1` until 
`Channel_16`. We'll ignore the cable number for now, if you don't specifically
set it, it'll just use the default cable.  
You can find more information about MIDI addresses in the
@ref midi_md-midi-addresses "MIDI Tutorial: MIDI addresses". 

For the MIDI controller numbers, you can use the 
@ref MIDI_CC "predefined constants", or you can just use a number.

```cpp
CCPotentiometer potentiometer { A1, {MIDI_CC::Channel_Volume, Channel_1} };
```
```cpp
CCPotentiometer potentiometer { A1, {7, Channel_1} };
```

In our case, we don't want a single potentiometer, we want eight. It's much
easier to define them in an array.  
Also note how we declare that the potentiometers are connected to the the pins 
of the multiplexer we defined in the previous step.

To go from the initialization of a single potentiometer to the initialization of
an array of potentiometers, we just create a comma-separated list of 
initializers for single elements, and then we add another pair of curly braces 
around it.  
Compare this to the initialization of an array of integers: to initialize a 
single integer, you use `int i = 42;` and to initialize an array of integers, 
you use `int ii[] = {42, 43, ...};`. 
Analogously, to initialize a single potentiometer object, you use 
`CCPotentiometer pot = {pin, address};` and to initialize an array of 
potentiometers, you use 
`CCPotentiometer pots[] = { {pin, address}, {pin, address}, ... };`. 
It is recommended to leave out the equal sign (`=`), because this will result
in more meaningful error messages if you get any of the arguments wrong:
`CCPotentiometer pots[] { {pin, address}, {pin, address}, ... };`. 

> **Note**: The first pin is of the multiplexer is `pin(0)`, not `pin(1)`.

```cpp
CCPotentiometer volumePotentiometers[] {
    { mux.pin(0), {MIDI_CC::Channel_Volume, Channel_1} },
    { mux.pin(1), {MIDI_CC::Channel_Volume, Channel_2} },
    { mux.pin(2), {MIDI_CC::Channel_Volume, Channel_3} },
    { mux.pin(3), {MIDI_CC::Channel_Volume, Channel_4} },
    { mux.pin(4), {MIDI_CC::Channel_Volume, Channel_5} },
    { mux.pin(5), {MIDI_CC::Channel_Volume, Channel_6} },
    { mux.pin(6), {MIDI_CC::Channel_Volume, Channel_7} },
    { mux.pin(7), {MIDI_CC::Channel_Volume, Channel_8} },
};
```

> **Note**: Some other MIDI Control Elements might not need an address number,
> or they might not need a channel. In that case, you just leave out the 
> optional parts that you don't need.  
> For example, a `PBPotentiometer` doesn't need an address number, just a 
> channel, so you can instantiate it as follows:
> ```cpp
> PBPotentiometer potentiometer { A1, Channel_9 };
> ```

### 5. Initialize the Control Surface                       {#first-output-init}

There's a lot to be done in the `setup`: The MIDI interface has to be 
initialized, all pins must be set to the correct mode, etc.  
Luckily, the Control Surface Library handles almost all of it for you.

```cpp
void setup() {
    Control_Surface.begin();
}
```

> **Note**: If you forget to define a MIDI interface, `Control_Surface.begin()`
> will raise an error and the on-board LED will start blinking.  
> Other errors will also be indicated this way.  
> If you don't know why this happens, you should enable debug information in 
> the `Control_Surface/src/AH/Settings/Settings.h` file, and inspect the output
> in the Serial Monitor.  
> Someday, I will add a "Troubleshooting" page. For now, if you have any 
> problems, just [open an issue on GitHub](https://github.com/tttapa/Control-Surface/issues/new)
> to remind me.

### 6. Continuously Update the Control Surface              {#first-output-loop}

Now that everything is set up, you can just update the Control Surface forever.
It will refresh all inputs and send the appropriate MIDI messages if any of the
inputs change.

```cpp
void loop() {
    Control_Surface.loop();
}
```

> **Note**: Using blocking code like delays will interfere with the Control
> Surface, so try to use [Blink Without Delay](https://www.arduino.cc/en/Tutorial/BlinkWithoutDelay) 
> techniques instead.

### The finished sketch

That's it!  
Now you can just upload the sketch to your Arduino, open up your favorite audio
software, map the potentiometers, and start playing!

```cpp
// Include the library
#include <Control_Surface.h>

// Instantiate a MIDI Interface to use
USBMIDI_Interface midi;

// Instantiate an analog multiplexer
CD74HC4051 mux {
  A0,       // Analog input pin
  {3, 4, 5} // Address pins S0, S1, S2
};

// Create an array of potentiometers that send out
// MIDI Control Change messages when you turn the
// potentiometers connected to the eight input pins of
// the multiplexer
CCPotentiometer volumePotentiometers[] {
  {mux.pin(0), {MIDI_CC::Channel_Volume, Channel_1}},
  {mux.pin(1), {MIDI_CC::Channel_Volume, Channel_2}},
  {mux.pin(2), {MIDI_CC::Channel_Volume, Channel_3}},
  {mux.pin(3), {MIDI_CC::Channel_Volume, Channel_4}},
  {mux.pin(4), {MIDI_CC::Channel_Volume, Channel_5}},
  {mux.pin(5), {MIDI_CC::Channel_Volume, Channel_6}},
  {mux.pin(6), {MIDI_CC::Channel_Volume, Channel_7}},
  {mux.pin(7), {MIDI_CC::Channel_Volume, Channel_8}},
};

// Initialize the Control Surface
void setup() {
  Control_Surface.begin();
}

// Update the Control Surface
void loop() {
  Control_Surface.loop();
}
```

## First Input: Getting Feedback from the MIDI Software           {#first-input}

Unlike the MIDI Controller library, the Control Surface library does support
MIDI input.  
This example shows how to use the `NoteLED` class to turn on and off LEDs
when MIDI Note On/Off messages are received.  
The example shows the use of a shift register to drive the LEDs, but you can of
course use any pins you want.

@htmlonly
<iframe width="560" height="315"
src="https://www.youtube.com/embed/u1IbYXPT6mM?loop=1" frameborder="0"
allow="accelerometer; autoplay; encrypted-media; gyroscope;
picture-in-picture" allowfullscreen></iframe>
@endhtmlonly

### 1. Include the library

Include the library so that you have access to all the classes and functions.

```cpp
#include <Control_Surface.h>
```

### 2. Instantiate a MIDI Interface

See @ref first-output-midi-interface "First Output: Instantiate a MIDI Interface".

```cpp
USBMIDI_Interface midi;
```

### 3. Add Extended Input/Output elements (optional)

See @ref first-output-extio "First Output: Add Extended Input/Output elements".

In this example, we'll use a 74HC595 8-bit serial in/parallel out shift 
register. This allows us to drive eight LEDs using just the SPI bus and a single
digital pin. You can daisy chain as many shift registers as you want, without
requiring any more pins.

Each of the eight outputs of the shift register can be connected to the anode of
an LED. Connect the cathodes to ground through a current-limiting resistor.

Connect the clock input (SH_CP or SRCLK) of the shift register to the Arduino's 
SCK pin, the serial data input (DS or SER) of the shift register to the 
Arduino's MOSI pin, and the latch pin (ST_CP or RCLK) of the shift register to
digital pin 10 of the Arduino. Connect the Output Enable pin (OE) of the shift
register to ground, and the Master Reset (MR) pin of the shift register to Vcc
to enable it.

```cpp
SPIShiftRegisterOut<8> sreg {
  SPI,      // SPI interface to use
  10,       // Latch pin (ST_CP)
  MSBFIRST, // Bit order
};
```

The `8` between angle brackets (`<>`) is the number of bits of the shift 
register. If you daisy chain two 8-bit shift registers together, you would use
`16` instead of `8`, for example.  
The bit order determines which pin of the shift register is the first pin in the
program. 
`MSBFIRST` means "most significant bit first". You can also use `LSBFIRST` 
(least significant bit first).

### 4. Add MIDI Control Elements

Now, we can specify the objects that listen for MIDI input, and update the 
status of the LEDs accordingly.

I'll refer to the overview of @ref MIDIInputElements.

Let's define a single LED on pin `13` that listens for MIDI Note events for
a middle C on channel 1.  
In the @ref NoteLED "documentation",
you'll find that the first argument for the `NoteLED` constructor is the 
number of the pin with the LED connected, and the second is the MIDI address.  
The MIDI address is a structure that consists of an address number, 
the MIDI channel, and the cable number.  
In this case, the address number is the note number, which is a number
from 0 to 127. The MIDI channel is a channel from `Channel_1` until 
`Channel_16`. We'll ignore the cable number for now, if you don't specifically
set it, it'll just use the default cable.  

For the MIDI note numbers, you can use the note constants and the `note`
function in the @ref MIDI_Notes namespace,
or you can just use a number.

```cpp
NoteLED noteLed { 13, {MIDI_Notes::C[4], Channel_1} };  // C4 = middle C
```

In our case, we don't want a single LED, we want eight. It's much easier to 
define them in an array.  
Also note how we state that it should use the pins of the shift register we 
defined in the previous step. We omit the channel here, so it'll just use the
default channel, `Channel_1`.

> **Note**: The first pin is `pin(0)`, not `pin(1)`.

```cpp
NoteLED leds[] {
  {sreg.pin(0), MIDI_Notes::C[4]},
  {sreg.pin(1), MIDI_Notes::D[4]},
  {sreg.pin(2), MIDI_Notes::E[4]},
  {sreg.pin(3), MIDI_Notes::F[4]},
  {sreg.pin(4), MIDI_Notes::G[4]},
  {sreg.pin(5), MIDI_Notes::A[4]},
  {sreg.pin(6), MIDI_Notes::B[4]},
  {sreg.pin(7), MIDI_Notes::C[5]},
};
```

### 5. Initialize the Control Surface

See @ref first-output-init "First Output: Initialize the Control Surface".

```cpp
void setup() {
  Control_Surface.begin();
}
```

### 6. Continuously Update the Control Surface

See @ref first-output-loop "First Output: Continuously Update the Control Surface".

```cpp
void loop() {
  Control_Surface.loop();
}
```

### The finished sketch

That's it!  
Now you can just upload the sketch to your Arduino, open up your favorite audio
software, redirect the MIDI output to the Arduino, and start playing!

```cpp
// Include the library
#include <Control_Surface.h>

// Instantiate a MIDI Interface to use
USBMIDI_Interface midi;

// Instantiate a shift register as output for the LEDs
SPIShiftRegisterOut<8> sreg {
  SPI,      // SPI interface to use
  10,       // Latch pin (ST_CP)
  MSBFIRST, // Byte order
};

// Create an array of LEDs that listen to MIDI Note messages, turning on and off
// the LEDs connected to the eight output pins of the shift register
NoteLED leds[] {
  {sreg.pin(0), MIDI_Notes::C[4]},  // LED pin, address (note number, channel, cable)
  {sreg.pin(1), MIDI_Notes::D[4]},  //
  {sreg.pin(2), MIDI_Notes::E[4]},  //
  {sreg.pin(3), MIDI_Notes::F[4]},  //
  {sreg.pin(4), MIDI_Notes::G[4]},  //
  {sreg.pin(5), MIDI_Notes::A[4]},  //
  {sreg.pin(6), MIDI_Notes::B[4]},  //
  {sreg.pin(7), MIDI_Notes::C[5]},  //
};

// Initialize the Control Surface
void setup() {
  Control_Surface.begin();
}

// Update the Control Surface
void loop() {
  Control_Surface.loop();
}
```

## Troubleshooting {#getting-started_md-troubleshooting}

If something goes wrong at run time, Control Surface will start blinking the 
on-board LED, with a pattern of two short flashes, followed by a longer pause, 
and a period of one second. You can enable debug output by defining the 
`DEBUG_OUT` macro in
`~Arduino/libraries/Control-Surface/src/AH/Settings/Settings.hpp` as `Serial`,
for example. This requires quite a bit of extra memory, so that might not be 
feasible on all boards, or you might have to simplify your sketch a bit.

If your sketch produces an error as soon as you call `Control_Surface.begin()`,
you might not be able to open the Serial Monitor quickly enough to catch the
error message, in that case, you can wait for the Serial connection to be opened
before initializing Control Surface. For example:

```cpp
#include <Control_Surface.h>
 
// Oops! We forgot to instantiate a MIDI interface!
 
NoteButton button {2, MIDI_Notes::C[4]};
NoteLED led {13, MIDI_Notes::C[4]};
 
void setup() {
#ifdef DEBUG_OUT
    DEBUG_OUT.begin(115200); // Initialize the debug output
    while (!DEBUG_OUT);      // and wait for it to become available
#endif
    Control_Surface.begin(); 
}
 
void loop() {
    Control_Surface.loop();
}
```
If you enable debug output, you'll get a helpful error message as soon as you 
open the Serial Monitor:
```text
[bool cs::Control_Surface_::connectDefaultMIDI_Interface() @ line 53]:	Fatal Error: No default MIDI Interface (0xF123)
```
The error code at the end can be used to easily locate the source of the error 
in the library source code, and in this case the message also includes the name
of the function and the line number where the error occurred.

> **Note**: Since the built-in LED will be configured as an output when an error
> occurs, you should not use it as an input (e.g. to connect a switch), because
> this will cause a short circuit that might destroy the pin.
