# Basics

[TOC]

## MIDI Addresses

All MIDI Input and Output Elements use MIDI addresses that determine the 
addresses of the MIDI messages they listen for, and the addresses of the 
messages they send.

### Address fields

The MIDI addresses used by the library have three different fields:

- The actual address, like the MIDI note number or the MIDI Control Change 
  controller number, a number between 0 and 127
- The MIDI Channel, between channel 1 and channel 16
- The MIDI USB Cable Number, a number between 0 and 15

The MIDI USB Cable Number is only used on boards that have support for multiple 
virtual MIDI cables over a single USB connection. As far as I know, only 
Teensies support this. USB-capable boards that use the MIDIUSB library do not
support multiple MIDI cables.  
Serial (5-pin DIN) MIDI and MIDI over Bluetooth LE have no notion of cables.

### Creating a MIDI address variable

The data type for MIDI addresses is @ref MIDIAddress. Click the link in the 
previous sentence, and have a look at the documentation. If you scroll down to 
the _Constructors_ section, you'll see that there are different ways to create
a @ref MIDIAddress.  
This is the most important one:

~~~cpp
MIDIAddress(int address, 
            Channel channel = CHANNEL_1,
            int cableNumber = 0x0)
~~~

It allows you to specify all three of the fields explained above. The channel 
and the cable number have default values, so these parameters are optional. If 
you don't specify the channel and the cable number, it will use the first 
channel on the first cable.

For example:
~~~cpp
// Address 16 on channel 1 and cable 0:
MIDIAddress myAddress = 16;

// Address 42 on channel 2 and cable 0:
MIDIAddress myAddress = {42, CHANNEL_2};

// Address 111 on channel 2 and cable 1:
MIDIAddress myAddress = {111, CHANNEL_2, 1};
~~~

If you specify just a single argument, as in the first example, you don't have
to use curly braces (`{}`). If you specify more than one argument, you need 
curly braces.

#### MIDI Control Change constants

Instead of specifying the controller number (address) as a number, you can also
use the predefined constants in the @ref MIDI_CC namespace:

~~~cpp
// Control Change General Purpose Controller #1 on MIDI Channel 2:
MIDIAddress myAddress = {MIDI_CC::General_Purpose_Controller_1, CHANNEL_2};
~~~

#### MIDI Note Numbers

For entering MIDI note numbers, you can use the note names in the 
@ref MIDI_Notes namespace and the @ref MIDI_Notes::note function:

~~~cpp
using namespace MIDI_Notes;
// MIDI Note middle C (C in the fourth octave) on MIDI Channel 16:
MIDIAddress myAddress = {note(C, 4), CHANNEL_16};
~~~

### Passing MIDI Addresses to MIDI Elements

Almost all @ref MIDIOutputElements and @ref MIDIInputElements take a 
@ref MIDIAddress as a parameter. Initializing the address is very similar to the
initialization introduced in the previous section: 
if you only want to specify the address, just pass it as a parameter, if you 
want to specify the channel or cable number as well, add a pair of curly braces 
around it.

For example:
~~~cpp
// Controller 16 on channel 1 and cable 0:
CCPotentiometer myPotentiometer = {
    A0, // pin number
    16, // MIDI address
};

// Controller 42 on channel 2 and cable 0:
CCPotentiometer myPotentiometer = {
    A0,              // pin number
    {42, CHANNEL_2}, // MIDI address
};

// Controller 111 on channel 2 and cable 1:
CCPotentiometer myPotentiometer = {
    A0,                  // pin number
    {111, CHANNEL_2, 1}, // MIDI address
};
~~~
