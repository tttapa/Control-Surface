# Frequently Asked Questions

[TOC]

## Terminology {#faq-terminology}

- **MIDI Output Element**: Any component of a MIDI controller that sends out 
MIDI messages (e.g. a push button or a potentiometer).
- **MIDI Input Element**: Any component of a MIDI controller that listens for 
incoming MIDI messages (e.g. an LED that indicates the “mute” or “solo” status 
of a track, or a display with a level meter that shows the level of the audio
playing in the DAW).
- **MIDI Element**: Broad term for both MIDI Output Elements and MIDI Input
Elements. It usually refers to both the actual hardware (e.g. a push button) and
the software object that reads the hardware and sends or receives the MIDI 
messages (e.g. a `NoteButton` object or variable in the Arduino code).

## Can I use Control Surface as a general purpose MIDI library?

Absolutely! Even though Control Surface has many high-level utilities for 
building MIDI controllers, at its core is a solid MIDI input/output system with
well-tested MIDI interfaces and MIDI parsers for many different transports, such
as the classic 5-pin DIN MIDI, MIDI over USB and MIDI over Bluetooth Low Energy. 
There's also a Debug MIDI interface that prints the MIDI messages to the serial
monitor, and there are wrappers for using third-party libraries such as 
AppleMIDI. See the @ref MIDIInterfaces module for a complete overview of the 
available MIDI interfaces.

### Sending MIDI

You can use the functions defined by the @ref MIDI_Sender class to send all 
kinds of MIDI messages. These functions can be used on all 
@ref MIDIInterfaces "MIDI interfaces" and on the main `Control_Surface` class.

Here's a basic MIDI output example:
```cpp
USBMIDI_Interface midi; // Instantiate the MIDI over USB interface

using namespace MIDI_Notes;
const MIDIAddress noteAddress = {note(C, 4), CHANNEL_1};
const uint8_t velocity = 0x7F;

void setup() {
  midi.begin();       // initialize the MIDI interface
}

void loop() {
  midi.sendNoteOn(noteAddress, velocity);         // send a note on event
  delay(1000);
  midi.sendNoteOff(noteAddress, velocity);        // send a note off event
  delay(1000);
}
```

### Receiving MIDI

For MIDI input, a class with callback functions for the different messages is
used. When an incoming MIDI message is available, `midi.update()` will call the
callbacks for you. See the @ref MIDI_Callbacks class and the examples listed
below.

### Routing MIDI

Apart from low-level MIDI input/output, you can also set up advanced MIDI 
routing rules between different interfaces. Two or more interfaces are connected
through @ref MIDI_Pipe "MIDI Pipes". The simplest pipe just carries messages 
from the input interface to the output interface, but you can write rules for 
filtering out certain messages, changing the channel of some messages, etc.  
For more information, see the @ref MIDI_Routing module.

### MIDI examples

 - @ref MIDI-Output.ino
 - @ref MIDI-Input.ino
 - @ref Send-MIDI-Notes.ino
 - @ref SysEx-Send-Receive.ino
 - @ref MIDI_Pipes-Routing.ino

### Combining low-level MIDI code and high-level “Control Surface” code

If need access to the incoming MIDI messages while also using the high-level
`Control_Surface` class, you can also add callbacks to `Control_Surface` itself.
You can then decide whether or not you want to pass on the the message to 
`Control_Surface` when you're done with it. See the @ref MIDI-Input-Callback.ino
example for more details.  
If you just want to use the low-level sending functions, you can either use the
member functions of `Control_Surface`, like 
`Control_Surface.sendNoteOn(note, velocity)`, or you can use the MIDI interfaces
directly.

## Can I use multiplexers for reading rotary encoders? {#faq-mux-encoder}

No, rotary encoders either need interrupts or have to be polled at a 
sufficiently high rate to work correctly, otherwise, you'll miss pulses when 
turning the encoder too quickly.
Multiplexers generally do not generate interrupts when the inputs change, and
polling them is often too slow. If your code contains other slow operations,
such as updating a display or reading many analog inputs, polling is no longer
an option.

If you can, try to connect rotary encoders to the Arduino directly, with at 
least one interrupt pin per encoder, preferably two.

If that's not an option because you're running out of pins, you can use a port 
expander, such as the MCP23017. These chips can be configured to generate an
interrupt whenever one of the inputs changes, and also have built-in pull-up 
resistors, which is handy.  
To read encoders with the MCP23017 in an interrupt-driven way, the 
microcontroller should support I²C inside of an ISR (Interrupt Service Routine),
in order to communicate with the port expander to check which pin changed.
Not all microcontrollers support this, but the ARM processors of the Teensy 3.x
and 4.x lines do.  
If I²C communication is not possible inside of an ISR (AVR Arduino boards, 
ESP32, etc.), you can still use polling, on the condition that the rest of your 
code doesn't take too long.

There are two examples for using rotary encoders with a MCP23017:
 - @ref MCP23017-RotaryEncoder-Interrupts.ino (Teensy)
 - @ref MCP23017-RotaryEncoder-No-Interrupts.ino (Other boards)

## Can I change the address, controller number, MIDI channel, etc. at runtime? {#faq-change-address-runtime}

Yes, this is done through banks. See the @ref Bank.ino example.  
The standard @ref BankableMIDIOutputElements have fixed offsets between the 
different addresses (e.g. address bank 1 = controller #8, bank 2 = controller 
#16, bank 3 = controller #24 and bank 4 = controller #32), while the 
@ref ManyAddressesMIDIOutputElements allow you to specify arbitrary addresses, 
they don't need to have a fixed offset between them.

## How do banks work? {#faq-banks}

There are two kinds of “Bankable” MIDI Elements: the standard `Bankable` 
elements and the `Bankable::ManyAddresses` elements.

### Normal banks

Normal `Bankable` elements allow you to change the address of the element by
multiples of a fixed offset. You can change the MIDI address (such as controller
number or note number), the MIDI channel or the MIDI USB virtual cable number
of the `Bankable` element.  
The formula for the address is a linear function of the active bank: 
@f[ y = ax + b, @f]
where @f$ y @f$ is the effective address, @f$ a @f$ is the 
fixed offset between banks, @f$ x @f$ is the currently selected bank 
(@f$ x=0 @f$ is the first bank), and @f$ b @f$ is the base address, which is the
address when the first bank is selected. The offset @f$ a @f$ is a property of
the Bank, and the base address @f$ b @f$ is a property of the `Bankable` MIDI 
element.

This formula is rather abstract, and it might not immediately be clear why this
is useful, so I'd recommend playing around some with the @ref Bank.ino example.

Another example is the following: imagine that you want to mute or un-mute any
of the 12 audio tracks in your DAW using only 4 physical buttons. In that case,
you could use 3 banks to change the addresses of the buttons. When the first
bank is selected, the first button mutes track 1, the second button mutes track
2, etc. When the second bank is selected, the first button mutes track 5, the 
second button mutes track 6, and so on.  
In this example, the offset @f$ a @f$ is 4, the number of buttons, or the number
of tracks you can control per bank.
The base address @f$ b @f$ of the first button is 1, for the second button, it's
2, for the third button, it's 3, and so on.

Click the buttons in the animation below to get a feel for how it works:

@htmlonly
<object type="image/svg+xml" data="../../bank-bankable.svg" style="margin: 0 auto; display: block"></object>
@endhtmlonly

```cpp
Bank<3> bank(4); // 3 banks with an offset of 4 tracks per bank (a = 4)

Bankable::NoteButton muteButtons[] {
    {bank, pin1, 1}, // base address 1 (b = 1)
    {bank, pin2, 2}, // base address 2 (b = 2)
    {bank, pin3, 3}, // base address 3 (b = 3)
    {bank, pin4, 4}, // base address 4 (b = 4)
};
```

### Many addresses

In most cases, the standard `Bankable` elements are sufficient, but sometimes, 
you might need a more flexible solution. The `Bankable::ManyAddresses` elements
allow you to specify an arbitrary list of alternative addresses. The formula for
the address is simply a table lookup in that list of addresses, based on the 
active bank:
@f[ y = \text{list of addresses}[x] @f].

## What's the difference between the Control Surface and MIDI Controller libraries?

You might already have found my other Arduino MIDI library, [MIDI Controller](https://github.com/tttapa/MIDI_Controller), 
and are wondering which one you should use for your project.

Short answer: Use Control Surface. MIDI Controller is no longer supported and 
Control Surface completely replaces every aspect of it.

For the long answer, first, some background:  
I first started working on the MIDI Controller library way back in 2015, and it
evolved a lot early on. The library seemed to be pretty popular, and it worked
pretty well, so I couldn't just push breaking changes every couple of months.  
Many people requested support for MIDI input, and I liked experimenting with it
as well. The main problem was that the overall architecture of the library 
needed a complete overhaul in order to add MIDI input support. Since I didn't 
know if the MIDI input was going to work out, and I didn't want to break 
compatibility with older versions of the library, I decided to fork it: Control
Surface was born.

I consider the MIDI Controller library obsolete. I won't be 
adding any new features, and I no longer offer support for it.  
For new projects, you should always use Control Surface, it is actively being 
developed, features are added, bugs are fixed, and I offer support for it on
the Arduino forum and in the GitHub issues.

The main difference between the two libraries is that Control Surface has much
more features. MIDI Controller has everything you need for a working MIDI 
controller with potentiometers, push buttons, rotary encoders, etc., while 
Control Surface supports all of that, plus MIDI input, LEDs, VU meters, OLED 
displays, MIDI over Bluetooth, Audio over USB, etc.  
Another major difference is the documentation and tests. Control Surface tries
to provide better documentation using Doxygen, and it has many unit tests to 
make sure I don't introduce any bugs.
