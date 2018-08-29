# Getting Started

## Installation

Instructions on how to install the library and its dependencies can be found 
in [Installation.md](Installation.md).

## Creating a Basic MIDI Controller Sketch

### 1. Including the library

Include the library so that you have access to all the classes and functions.

```cpp
#include <Control_Surface.h>
```

### 2. Instantiate a MIDI Interface

If you want to send out or receive MIDI messages, you have to define at least 
one MIDI interface. If you don't do that, you'll get an error when calling
`Control_Surface.begin()`.

There are many different MIDI interfaces to choose from:

- `USBMIDI_Interface`: On boards that support MIDI over USB natively, this will
do exactly what you'd expect. You just have to plug it into your computer, and
it shows up as a MIDI device.  
On Arduinos without native USB capabilities (e.g. UNO or MEGA), you have to use 
a custom firmware.
- `HardwareSerialMIDI_Interface`: This interface will send and receive MIDI 
messages over a hardware UART. You can use it for MIDI over a 5-pin din 
connector, for example.
- `HairlessMIDI_Interface`: If you have an Arduino without native USB support, 
an easy way to get the MIDI messages from your Arduino to your computer is 
the [Hairless MIDI<->Serial Bridge](https://projectgus.github.io/hairless-midiserial/).
In that case, you can use this MIDI interface. The default baud rate is 115200
symbols per second.
- `BluetoothMIDI_Interface`: If you have an ESP32, you can send and receive MIDI
messages over Bluetooth Low Energie. This interface is still very much
experimental, but it's pretty cool. If you know more about the MIDI BLE
protocol, feel free to suggest some improvements.
- `USBDebugMIDI_Interface`: Debugging MIDI Controllers can be very cumbersome. 
There are MIDI monitors available, but you have to reconnect every time you 
upload a new sketch, and sending MIDI to the Arduino is not always easy.  
This interface is designed to help you with that. It prints outgoing MIDI
messages to the Serial port in a readable format, and it allows you to enter 
hexadecimal MIDI messages (as text) in the Serial monitor 
(e.g. `90 3C 7F` to turn on middle C).

A complete overview of the available MIDI interfaces can be found [here](
https://tttapa.github.io/Control-Surface/Doc/Doxygen/dc/df0/group__MIDIInterfaces.html).

For now, we'll use the `USBMIDI_Interface` because it's probably the one you'll
use in your final program.

You can give the interface any name you want, here, I'll be very original and
choose `midi`. It doesn't matter, and you don't need to use it afterwards, 
defining the interface is enough, the Control Surface library will automatically
know that it exists.

```cpp
USBMIDI_Interface midi;
```

As a quick note: some interfaces require additional parameters, for example, 
the `USBDebugMIDI_Interface` needs to know the baud rate.  
In that case, you can instantiate it as follows:

```cpp
USBDebugMIDI_Interface midi = { 115200 };
```

### 3. Add Extended Input/Output elements (optional)

If your MIDI Controller requires many in- or outputs, you'll run out of IO pins
really quickly. A solution is to use multiplexers or shift registers.  
The Control Surface Library supports both of these options, and makes it easy
to support other types of IO expanders in the future.

An overview of Extended Input/Output elements can be found [here](
https://tttapa.github.io/Control-Surface/Doc/Doxygen/d1/d8e/group__ExtIO.html).

In this example, we'll use an 8-channel CD74HC4051 analog multiplexer. This
allows us to read eight analog inputs using only one analog pin on the Arduino,
at the cost of only three digital output pins.

Each of the eight analog inputs of the multiplexer can be connected to the wiper
of one potentiometer.

We'll connect the three address lines of the multiplexer (`S0`, `S1` and `S2`)
to digital pins `10`, `11` and `12`. The output of the multiplexer goes to 
analog pin `A0`. Connect the enable pin (`Ē`) to ground.

```cpp
CD74HC4051 mux = { A0, {10, 11, 12} };
```

### 4. Add MIDI Control Elements

Now we can specify the objects that read the input of the from the 
potentiometers and send out MIDI events.

Again, I'll refer to the overview of MIDI Output Elements [here](
https://tttapa.github.io/Control-Surface/Doc/Doxygen/d7/dcd/group__MIDIOutputElements.html).

Let's define a single potentiometer on pin `A1` that sends out MIDI Control 
Change events.  
In the [documentation](
https://tttapa.github.io/Control-Surface/Doc/Doxygen/db/d32/classCCPotentiometer.html),
you'll find that the first argument for the `CCPotentiometer` class is the 
analog pin number, the second is the MIDI controller number, and the third
is the MIDI channel.  
For the MIDI controller numbers, you can use the [predefined constants](
https://tttapa.github.io/Control-Surface/Doc/Doxygen/d4/dbe/namespaceMIDI__CC.html).

```cpp
CCPotentiometer potentiometer = { A1, MIDI_CC::Channel_Volume, 1 };
```

In our case, we don't want a single potentiometer, we want eight. It's much
easier to define them in an array.  
Also note how we state that it should use the pins of the multiplexer we 
defined in the previous step. 

> **Note**: The first pin is `pin(0)`, not `pin(1)`.

```cpp
CCPotentiometer volumePotentiometers[] = {
    { mux.pin(0), MIDI_CC::Channel_Volume, 1 },
    { mux.pin(1), MIDI_CC::Channel_Volume, 2 },
    { mux.pin(2), MIDI_CC::Channel_Volume, 3 },
    { mux.pin(3), MIDI_CC::Channel_Volume, 4 },
    { mux.pin(4), MIDI_CC::Channel_Volume, 5 },
    { mux.pin(5), MIDI_CC::Channel_Volume, 6 },
    { mux.pin(6), MIDI_CC::Channel_Volume, 7 },
    { mux.pin(7), MIDI_CC::Channel_Volume, 8 },
};
```

### 5. Initialize the Control Surface

There's a lot to be done in the `setup`: The MIDI interface has to be 
initialized, all pins must be set to the correct mode, etc.  
Luckily, the Control Surface Library
handles most of it for you.

```cpp
void setup() {
    Control_Surface.begin();
}
```

> **Note**: If you didn't define a MIDI interface, `Control_Surface.begin()`
> will raise an error and the on-board LED will start blinking.  
> Other errors will also be indicated this way.  
> If you don't know why this happens, you should enable debug information in 
> the `Control_Surface/src/Settings/Settings.h` file, and inspect the output
> in the Serial Monitor.

### 6. Continuously Update the Control Surface

Now that everything is set up, you can just update the Control Surface forever.
It will just refresh all inputs and send the appropriate MIDI messages if an
input changes.

```cpp
void loop() {
    Control_Surface.loop();
}
```

> **Note**: Using blocking code like delays in your code will interfere with the
> Control Surface, so try to use [Blink Without Delay](
https://www.arduino.cc/en/Tutorial/BlinkWithoutDelay) techniques instead.

### The finished sketch

That's it!  
Now you can just upload the sketch to your Arduino, open up your favourite audio
software, map the potentiometers, and start playing!

```cpp
#include <Control_Surface.h>

USBMIDI_Interface midi;

CD74HC4051 mux = { A0, {10, 11, 12} };

CCPotentiometer volumePotentiometers[] = {
    { mux.pin(0), MIDI_CC::Channel_Volume, 1 },
    { mux.pin(1), MIDI_CC::Channel_Volume, 2 },
    { mux.pin(2), MIDI_CC::Channel_Volume, 3 },
    { mux.pin(3), MIDI_CC::Channel_Volume, 4 },
    { mux.pin(4), MIDI_CC::Channel_Volume, 5 },
    { mux.pin(5), MIDI_CC::Channel_Volume, 6 },
    { mux.pin(6), MIDI_CC::Channel_Volume, 7 },
    { mux.pin(7), MIDI_CC::Channel_Volume, 8 },
};

void setup() {
    Control_Surface.begin();
}

void loop() {
    Control_Surface.loop();
}
```