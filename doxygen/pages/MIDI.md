# MIDI Tutorial {#midi-tutorial}

[TOC]

## The MIDI protocol

This guide expects basic familiarity with the MIDI protocol and the different 
types of MIDI messages.
You can find more information about MIDI on <https://www.midi.org/>, 
the [summary of MIDI messages](https://www.midi.org/specifications-old/item/table-1-summary-of-midi-message)
can be especially handy.

## MIDI interfaces {#midi_md-midi-interfaces}

Control Surface supports sending and receiving MIDI messages in many different 
ways: over a standard 5-pin DIN MIDI cable, over USB, over Bluetooth, over WiFi
...  
Each of those methods of transport is available as a “MIDI Interface” class in
the code, you can find a full overview in the @ref MIDIInterfaces module.

The interfaces you're most likely to use are:
- @ref USBMIDI_Interface : the Arduino will be recognized as a MIDI USB device 
    when you plug it into your computer, and this interface allows you to send 
    and receive MIDI messages to and from applications running on the computer.
- @ref USBDebugMIDI_Interface : prints all messages to the Arduino Serial Monitor 
    in a human-readable format, and allows you to easily check that your code 
    sends the correct messages. Also allows you to send messages to the Arduino
    from the Serial Monitor.
- @ref HardwareSerialMIDI_Interface : sends and receives MIDI over the TX and RX
    pins of the Arduino, can be used with standard 5-pin DIN MIDI.
- @ref BluetoothMIDI_Interface : makes the Arduino advertise itself as a
    Bluetooth Low Energy (BLE) MIDI peripheral, allowing you to send and receive
    MIDI messages wirelessly.

Other available interfaces are @ref HairlessMIDI_Interface,
@ref SoftwareSerialMIDI_Interface, @ref USBHostMIDI_Interface ...

### Supported Arduino-compatible boards {#midi_md-interfaces-supported-boards}

Not all MIDI interfaces are supported on all Arduino boards. For example, not 
all Arduino boards support MIDI over USB natively. You can find an overview 
of boards that do support it on the @ref md_pages_MIDI-over-USB page.
USB Host MIDI is currently only supported on the Teensy 3.6 and 4.1 boards.

MIDI over BLE is supported on the ESP32, the Raspberry Pi Pico W, and on boards
supported by the ArduinoBLE library. More information can be found on the
@ref md_pages_MIDI-over-BLE page.

### Functionality {#midi_md-interfaces-functionality}

The MIDI interfaces provide the following functionality:
- **Sending MIDI**: You can send MIDI messages from your Arduino code to 
whatever device is connected on the other side of the interface.
- **Receiving MIDI**: MIDI messages sent by the device on the other side can 
be read and inspected in your code, or you can register a callback that gets 
called whenever a message arrives through the interface.
- **Routing MIDI**: %Interfaces can be configured to automatically route MIDI
messages from one interface to others, allowing you to filter or modify the
messages as they travel between interfaces.

In the remainder of this tutorial, one section will be devoted to each of these
functions.

### Instantiating, initializing and updating a MIDI interface {#midi_md-instantiate-a-midi-interface}

The following code snippet demonstrates how to instantiate, initialize, and 
update a MIDI over USB interface. By “instantiate”, we mean that a variable is 
created that is an _instance_ of the `USBMIDI_Interface` type.

```cpp
#include <Control_Surface.h>

// Instantiate a MIDI over USB interface with the name `midi`
USBMIDI_Interface midi;
// `midi` is the name of a variable of type `USBMIDI_Interface`

void setup() {
    // Initialize the MIDI interface so it's ready to be used
    midi.begin();
}

void loop() {
    // Continuously listen for incoming MIDI messages
    midi.update();
}
```
Note how the words `%USBMIDI_Interface`, `begin` and `update` in the previous
snippet are shown in different colors: 
these are links to the documentation for the relevant
classes and functions. If you hover over them with your mouse pointer, you'll 
see a short description, and if you click on the link, it will take you to the 
detailed documentation.

The code above doesn't really do anything useful yet, it just sets up everything
so we can start sending and receiving MIDI messages in the following sections.

You can of course replace `%USBMIDI_Interface` by a different type, such as 
`%USBDebugMIDI_Interface` if that's the MIDI interface you need for your 
application.
```cpp
USBDebugMIDI_Interface midi;
```
Some classes allow you to specify extra parameters: for example, 
`%USBDebugMIDI_Interface` accepts an optional argument that 
specifies the baud rate to use when communicating with the Serial Monitor.
The possible parameters can be found in the documentation for the _constructor_
of the class in question, for example, see 
@ref USBDebugMIDI_Interface::USBDebugMIDI_Interface(), it lists `baud` as an 
optional parameter. You can see that it's an optional parameter, because its 
name is followed by an equal sign (`=`) and a default value in the function 
signature.

To specify the baud rate using this parameter, you can replace the declaration
of the `midi` variable in the previous snippet by the following:
```cpp
// Instantiate a debug MIDI interface with a baud rate of 9600
USBDebugMIDI_Interface midi {9600};
```
Other interfaces might require multiple parameters. Consider 
`%HardwareSerialMIDI_Interface` as an example. Have a look at the documentation
of its constructor 
@ref HardwareSerialMIDI_Interface::HardwareSerialMIDI_Interface(), you'll see 
that the first required parameter is the serial port to use, and the second
(optional) parameter is the baud rate.

For example:
```cpp
// Instantiate a hardware MIDI interface with Serial1 as its UART,
// using the default baud rate
HardwareSerialMIDI_Interface midi {Serial1};
```
```cpp
// Instantiate a hardware MIDI interface with Serial1 as its UART,
// using a baud rate of 115200
HardwareSerialMIDI_Interface midi {Serial1, 115200};
```

## Sending MIDI messages {#midi_md-sending}

The functions for sending MIDI messages are all listed in the @ref MIDI_Sender 
class. These functions are available for all MIDI interfaces.

### Simple example {#midi_md-sending-simple-example}

First, we'll give a simple example that just triggers a MIDI note every second:

```cpp
#include <Control_Surface.h>
 
// Instantiate a MIDI over USB interface
USBMIDI_Interface midi;
 
void setup() {
    midi.begin(); // Initialize the MIDI interface
}
 
// Specify the MIDI note number to trigger, and the velocity with which to 
// trigger it
const MIDIAddress note = MIDI_Notes::C[4]; // C4 is middle C
const uint8_t velocity = 127;              // 127 is maximum velocity
 
void loop() {
    // Send a MIDI Note On message for the given note to trigger it
    midi.sendNoteOn(note, velocity);  
    delay(500);
    // Send a MIDI Note Off message to turn it off again
    midi.sendNoteOff(note, velocity);
    delay(500);
    
    midi.update();
}
```

First of all, compare this example to the code under 
@ref midi_md-instantiate-a-midi-interface :
you'll see that the instantiation, initialization and updating of the MIDI
interface are exactly the same.

The second thing you'll notice are the two constants, `note` and `velocity`: 
these define the MIDI note number and the velocity (how hard the key is struck)
to be used when sending the MIDI messages later.  
The expression `MIDI_Notes::C[4]` gives you the note C in the fourth octave, or 
middle C. The velocity is a value between 1 and 127, with higher values being
louder and harder.

The `MIDIAddress` data type is one of the most important types in the library, 
it specifies the destination of each MIDI message: in this case, it specifies 
the note we want to trigger. In a following section, we'll look into 
`%MIDIAddress` in a bit more detail.

In the loop function, the `note` and `velocity` constants are then used to 
actually send the MIDI messages. This is done by calling the `sendNoteOn()` and
`sendNoteOff()` member functions of the MIDI interface. You can find the 
documentation in the @ref MIDI_Sender class mentioned earlier, or you can click
the brown links in the code snippet.

### MIDI addresses {#midi_md-midi-addresses}

To send a MIDI note message, you need to know the MIDI note number of the
note you want to trigger, the MIDI channel to send it to, and the virtual cable 
to send it over.
These three pieces of information are stored by the `MIDIAddress` type.

For MIDI messages other than Note On/Off, the note number might be replaced by
a different type of address, such as the controller number for Control Change 
messages, or the patch/program number for Program Change messages, but the 
format remains the same:

- The actual address, like the MIDI note number or the MIDI Control Change 
  controller number, a number between 0 and 127
- The MIDI Channel, between channel 1 and channel 16
- The MIDI USB Virtual Cable Number, between cable 1 and cable 16

The MIDI USB Cable Number is only used on boards that have support for multiple 
virtual MIDI cables over a single USB connection. As far as I know, only 
Teensies support this. USB-capable boards that use the MIDIUSB library do not
support multiple MIDI cables.  
Serial (5-pin DIN) MIDI and MIDI over Bluetooth LE have no notion of cables.

#### Creating a MIDI address variable {#midi_md-creating-a-midi-address-variable}

Have a look at the documentation of @ref MIDIAddress. (You can click the link
in the previous sentence). 
If you scroll down to the _Constructors_ section,
you'll see that there are different ways to create a @ref MIDIAddress.  
This is the most important one: 
@ref MIDIAddress::MIDIAddress(int, Channel, Cable)

~~~cpp
MIDIAddress(int address, 
            Channel channel = Channel_1,
            Cable cableNumber = Cable_1)
~~~

It allows you to specify all three of the fields explained above. The channel 
and the cable number have default values, so these parameters are optional. If 
you don't specify the channel and the cable number, it will use the first 
channel on the first cable.

For example:
~~~cpp
// Address 16 on channel 1 and cable 1:
MIDIAddress myAddress = 16;

// Address 42 on channel 2 and cable 1:
MIDIAddress myAddress = {42, Channel_2};

// Address 111 on channel 2 and cable 3:
MIDIAddress myAddress = {111, Channel_2, Cable_3};
~~~

If you specify just a single argument, as in the first example, you don't have
to use curly braces (`{}`). If you specify more than one argument, you need 
curly braces.

#### MIDI Note names {#midi_md-midi-note-names}

For entering MIDI note numbers, you can use the note names in the 
@ref MIDI_Notes namespace:
~~~cpp
// MIDI Note middle C (C in the fourth octave) on MIDI Channel 16:
MIDIAddress myAddress = {MIDI_Notes::C[4], Channel_16};
~~~

If you look at the full list of note names in the @ref MIDI_Notes namespace, 
you'll see that the sharps are missing: you can simply use the flats instead 
(e.g. instead of using G♯, you can use A♭).

#### MIDI Control Change names {#midi_md-midi-control-change-names}

Instead of specifying the controller number (address) as a number, you can also
use the predefined constants in the @ref MIDI_CC namespace:
~~~cpp
// Control Change General Purpose Controller #1 on MIDI Channel 2:
MIDIAddress myAddress = {MIDI_CC::General_Purpose_Controller_1, Channel_2};
~~~

#### MIDI Program Change names {#midi_md-midi-program-change-names}

Similar to the constants for controller numbers, you can find the constants for 
all General MIDI program numbers in the @ref MIDI_PC namespace:
~~~cpp
// Harpsichord voice on MIDI Channel 9.
MIDIAddress myAddress = {MIDI_PC::Harpsichord, Channel_9};
~~~

### More examples {#midi_md-sending-more-examples}

- @ref MIDI-Output.ino
- @ref Send-MIDI-Notes.ino
- @ref Send-All-MIDI-Messages.ino
- @ref SysEx-Send-Receive.ino

## Receiving MIDI messages {#midi_md-receiving}

MIDI input is usually handled using callback functions. Whenever the MIDI 
interface receives a MIDI message, it calls the respective callback function
that was specified by the user.

You can either use a callback for each specific message type (Note On, Note Off,
Control Change ...), or a callback for each category of messages (%Channel
messages, System Exclusive message, System Common messages, Real-Time messages).

A third way is to poll the MIDI input manually, but this is not covered in this 
guide.

### Simple example {#midi_md-receiving-simple-example}

The @ref FineGrainedMIDI_Callbacks class allows you to associate a callback
function with each specific MIDI message type. The contents of the MIDI message
(such as the channel, note number, velocity, etc.) are passed to the callback
as arguments.
For example:
```cpp
#include <Control_Surface.h>
 
// Instantiate a MIDI over USB interface
USBMIDI_Interface midi;

// Custom MIDI callback that prints incoming MIDI note messages.
struct MyMIDI_Callbacks : FineGrainedMIDI_Callbacks<MyMIDI_Callbacks> {

    // Function that is called whenever a MIDI Note Off message is received.
    void onNoteOff(Channel channel, uint8_t note, uint8_t velocity, Cable cable) {
        Serial << "Note Off: " << channel << ", note " << note << ", velocity "
               << velocity << ", " << cable << endl;
    }

    // Function that is called whenever a MIDI Note On message is received.
    void onNoteOn(Channel channel, uint8_t note, uint8_t velocity, Cable cable) {
        Serial << "Note On: " << channel << ", note " << note << ", velocity "
               << velocity << ", " << cable << endl;
    }

} callback; // Instantiate a callback
 
void setup() {
    Serial.begin(115200);        // For printing the messages
    midi.begin();                // Initialize the MIDI interface
    midi.setCallbacks(callback); // Attach the custom callback
}
 
void loop() {
    midi.update(); // Continuously dispatch incoming MIDI input to the callbacks
}
```

The instantiation, initialization, and updating of the MIDI interface is the 
same as before. The first new component is the `MyMIDI_Callbacks` class: this 
class inherits from the @ref FineGrainedMIDI_Callbacks class that is provided by
the library, and implements two of the callback functions, 
@ref FineGrainedMIDI_Callbacks::onNoteOff() and 
@ref FineGrainedMIDI_Callbacks::onNoteOn().  
An instance of the `MyMIDI_Callbacks` class is created, and then this callback 
is attached to the MIDI interface in the `setup` function using 
@ref MIDI_Interface::setCallbacks().

After the callback has been attached, every time you call `midi.update()` in the 
`loop`, the MIDI interface will check if any new messages have arrived, and if
that's the case, it will call the corresponding callback function. For example,
whenever a MIDI Note On message arrives, the `onNoteOn()` function of the
callback object is called.

The example above is similar to the @ref MIDI-Input-Fine-Grained.ino example. 
You can find the full list of available callbacks and their arguments in the 
documentation for @ref FineGrainedMIDI_Callbacks, under 
@ref FineGrainedMIDI_Callbacks_MIDI_Callback_Functions "MIDI Callback Functions",
and in the @ref MIDI-Input-Fine-Grained-All-Callbacks.ino example.

### Customizable callbacks {#midi_md-receiving-customizable-callbacks}

While the fine-grained callbacks can be useful to easily handle most incoming
MIDI messages, sometimes you need more flexibility. It's not always desirable
to have an individual callback for each message type. The @ref MIDI_Callbacks
class allows you to specify a callback for each category of MIDI messages:
%Channel messages, System Exclusive messages, 
System Common messages and Real-Time messages. For example:

```cpp
#include <Control_Surface.h>
 
// Instantiate a MIDI over USB interface
USBMIDI_Interface midi;
 
// Custom MIDI callback that prints a simple message
struct MyMIDI_Callbacks : MIDI_Callbacks {
 
    // Callback for channel messages (notes, control change, pitch bend, etc.).
    void onChannelMessage(MIDI_Interface &, ChannelMessage) override {
        Serial.println("Channel message received");
    }
 
} callback; // Instantiate a callback
 
void setup() {
    Serial.begin(115200);        // For printing the messages
    midi.begin();                // Initialize the MIDI interface
    midi.setCallbacks(callback); // Attach the custom callback
}
 
void loop() {
    midi.update(); // Continuously dispatch incoming MIDI input to the callbacks
}
```

The first argument of the callback function is a reference to the MIDI interface
that received the message (this is not used in this simple example), and the 
second argument is the message itself.

### Accessing the contents of the message {#midi_md-accessing-the-contents-of-the-message}

The previous example simply printed “Channel message received” whenever it 
received a channel message. This is not really that useful, so in this section
we'll improve upon it by inspecting the contents of the messages and extracting
the different components such as the channel, note number, velocity, and so on.

Have a look at the documentation for @ref ChannelMessage.
You'll notice that the message contains four fields: 
`header`, `data1`, `data2` and `cable`. These correspond to the contents of 
channel messages as specified by the MIDI standard. Apart from these fields, 
there are also many utility functions to access different parts of the message,
such as `getMessageType()` which will tell you what type of message it is 
(e.g. Note On, Note Off, Control Change), `getChannel()` which extracts the MIDI
channel number from the header, `hasTwoDataBytes()` which tells you whether the 
message contains two data bytes (such as Note and Control Change messages) or 
just a single data byte (such as Program Change).

The following callback example simply checks how many bytes the message has, and
then prints the contents to the serial port in hexadecimal.

```cpp
// Custom MIDI callback that prints incoming MIDI channel messages.
struct MyMIDI_Callbacks : MIDI_Callbacks {
 
    // Callback for channel messages (notes, control change, pitch bend, etc.).
    void onChannelMessage(MIDI_Interface &, ChannelMessage msg) override {
        // Print the message
        Serial << F("Received: ") << hex << msg.header << ' ' << msg.data1;
        if (msg.hasTwoDataBytes()) 
            Serial << ' ' << msg.data2;
        Serial << dec << endl;
    }
 
} callback; // Instantiate a callback
```

If you send a Note On message on MIDI %Channel 4 for 
note number 60 = 0x3C (middle C) with a velocity of 
127 = 0x7F to the Arduino, you should see the following in the Serial monitor:
```text
Received: 93 3c 7f
```

### Listening for specific messages {#midi_md-listening-for-specific-messages}

In the following example, we'll create a callback that listens for MIDI Note On 
and Note Off messages specifically.

```cpp
// Custom MIDI callback that prints incoming MIDI note messages.
struct MyMIDI_Callbacks : MIDI_Callbacks {
 
    // Callback for channel messages (notes, control change, pitch bend, etc.).
    void onChannelMessage(MIDI_Interface &, ChannelMessage msg) override {
        auto type = msg.getMessageType();
        if (type == msg.NOTE_ON || type == msg.NOTE_OFF)
            onNoteMessage(msg);
    }

    // Our own callback specifically for Note On and Note Off messages
    void onNoteMessage(ChannelMessage msg) {
        auto type     = msg.getMessageType(); 
        auto channel  = msg.getChannel();
        auto note     = msg.getData1();
        auto velocity = msg.getData2();
        Serial << type << ": "
               << channel << ", Note " << note << ", Velocity " << velocity 
               << endl;
    }
 
} callback; // Instantiate a callback
```

If you send MIDI note messages to the Arduino, you should see output like this
in the Serial monitor:
```text
NOTE_ON: Channel 1, Note 18, Velocity 112
NOTE_OFF: Channel 1, Note 18, Velocity 127
```

You can go even further, for example, matching only a specific note on a 
specific MIDI channel:
```cpp
// Custom MIDI callback that prints incoming MIDI note messages.
struct MyMIDI_Callbacks : MIDI_Callbacks {
 
    // Callback for channel messages (notes, control change, pitch bend, etc.).
    void onChannelMessage(MIDI_Interface &, ChannelMessage msg) override {
        auto type     = msg.getMessageType(); 
        auto channel  = msg.getChannel();
        auto note     = msg.getData1();
        auto velocity = msg.getData2();
        // Check if it's a note message:
        bool note_msg = type == msg.NOTE_ON || type == msg.NOTE_OFF;
        // Check if it's the specific note and channel we're looking for
        bool our_note = channel == Channel_1 && note == MIDI_Notes::C[4];
        // If both conditions are satisfies, print the message
        if (note_msg && our_note)
            Serial << type << " middle C on Channel 1 with velocity "
                   << velocity << endl;
    }
 
} callback; // Instantiate a callback
```
The snippet above only prints output if it receives a Note On/Off event for 
middle C on MIDI channel 1. All other incoming MIDI messages are ignored. 
Possible output in the Serial monitor could be:
```text
NOTE_ON middle C with velocity 120
NOTE_OFF middle C with velocity 127
```

The previous example can be simplified by using the `MIDIAddress` type like 
in the previous section, and extracting the address from the incoming note 
message using the @ref ChannelMessage::getAddress() function:
```cpp
// Custom MIDI callback that prints incoming MIDI note messages.
struct MyMIDI_Callbacks : MIDI_Callbacks {
 
    // Callback for channel messages (notes, control change, pitch bend, etc.).
    void onChannelMessage(MIDI_Interface &, ChannelMessage msg) override {
        auto type     = msg.getMessageType(); 
        auto velocity = msg.getData2();
        // Check if it's a note message:
        bool note_msg = type == msg.NOTE_ON || type == msg.NOTE_OFF;
        // Define the address we're interested in
        const MIDIAddress our_special_note = {MIDI_Notes::C[4], Channel_1};
        // If it's a note message that matches our specific address, print it
        if (note_msg && msg.getAddress() == our_special_note)
            Serial << type << " middle C on Channel 1 with velocity "
                   << velocity << endl;
    }
 
} callback; // Instantiate a callback
```

### More examples {#midi_md-receiving-more-examples}

- @ref MIDI-Input.ino
- @ref MIDI-Input-Fine-Grained.ino
- @ref MIDI-Input-Fine-Grained-All-Callbacks.ino
- @ref SysEx-Send-Receive.ino

## Routing MIDI messages {#midi_md-routing}

### Default routing {#midi_md-default-routing}

Let's start with a very simple example:

```cpp
#include <Control_Surface.h>

// Instantiate a MIDI over USB interface with the name `midi_usb`
USBMIDI_Interface midi_usb;

NoteButton button {2, MIDI_Notes::C[4]};
NoteLED led {13, MIDI_Notes::C[4]};

void setup() {
    // Initialize everything, including MIDI interfaces and default routes
    Control_Surface.begin(); 
}

void loop() {
    Control_Surface.loop();
}
```
When you call `Control_Surface.begin()`, it will automatically create a default
route for you, which is shown in the following figure.

<img style="margin: 0 auto; display: block; width: 38em" src="midi-pipes-default.svg"/>

Two pipes are created, one for MIDI output (`pipe_tx` in the figure), and one 
for MIDI input (`pipe_rx`). These pipes are used to connect the 
`Control_Surface` part of the code to the MIDI interface defined on line 4
(`midi_usb`).

In the main loop of the program, `Control_Surface` checks if the button was 
pressed, and if that's the case, a MIDI message is sent by `Control_Surface`:
The message travels over the MIDI output pipe (`pipe_tx`) to the MIDI interface
(`midi_usb`), which will then send it to the computer over the USB connection.

Similarly, when a MIDI message from the computer arrives on the USB connection,
the MIDI interface (`midi_usb`) will send it over the MIDI input pipe 
(`pipe_rx`) to the `Control_Surface` code, which will then check whether it's a
MIDI Note On/Off message that matches the address of the LED, and turn on/off
the LED accordingly.

If you have multiple MIDI interfaces, you should explicitly set the default
interface in your `setup` function before calling `Control_Surface.begin()`,
using the @ref MIDI_Interface::setAsDefault() function:

```cpp
USBMIDI_Interface midi_usb;
HardwareSerialMIDI_Interface midi_din {Serial1};

void setup() {
    midi_usb.setAsDefault();
    Control_Surface.begin();
}
```

### Manual routing {#midi_md-manual-routing}

As a first exercise, we'll define the same pipes and routing ourselves, without 
relying on `Control_Surface.begin()` to do it automagically.

```cpp
#include <Control_Surface.h>

// Instantiate a MIDI over USB interface
USBMIDI_Interface midi_usb;
// Instantiate the two pipes to connect the interface to Control_Surface
MIDI_Pipe pipe_tx, pipe_rx;

NoteButton button {2, MIDI_Notes::C[4]};
NoteLED led {13, MIDI_Notes::C[4]};

void setup() {
    // Manually route MIDI output from Control_Surface to the MIDI interface
    Control_Surface >> pipe_tx >> midi_usb;
    // Manually route MIDI output from the MIDI interface to Control_Surface
    Control_Surface << pipe_rx << midi_usb;
    // Initialize everything
    Control_Surface.begin();
}

void loop() {
    Control_Surface.loop();
}
```
As you can see, the shift operators `>>` and `<<` are used to define the routes
between Control Surface and the MIDI interface. Note how they correspond to the
arrows in the figure above.

You can specify the two endpoints in any order you like (left-to-right or
right-to-left). For example, the following two lines of code are equivalent and 
define the same connection: 
```cpp
Control_Surface << pipe_rx << midi_usb;
midi_usb >> pipe_rx >> Control_Surface;
```

It is important to specify the routing _before_ you call 
`Control_Surface.begin()`. In that case, Control Surface detects that it is 
already connected to a MIDI interface, so it won't change anything to the manual
routing we specified earlier.  
If you call `Control_Surface.begin()` first, it will create its default routing
as discussed earlier, and if you then later add your manual connections on top
of that, your code might not work as expected.

### Bidirectional MIDI pipes {#midi_md-bidir-pipes}

There are many situations where you want to connect both the MIDI input and 
output of two endpoints together. It's a bit cumbersome to have to define a 
separate connection for each of the two directions. Luckily, the library 
includes bidirectional MIDI pipes, which makes this process a bit more terse.

The following sketch does exactly the same as the previous two, just using a
bidirectional pipe as a shorthand.

```cpp
#include <Control_Surface.h>

// Instantiate a MIDI over USB interface
USBMIDI_Interface midi_usb;
// Instantiate the pipe to connect the interface to Control_Surface
BidirectionalMIDI_Pipe pipe_txrx;

NoteButton button {2, MIDI_Notes::C[4]};
NoteLED led {13, MIDI_Notes::C[4]};

void setup() {
    // Manually route MIDI output from Control_Surface to the MIDI interface,
    // and the MIDI output from the MIDI interface to Control_Surface
    Control_Surface | pipe_txrx | midi_usb;
    // Initialize everything
    Control_Surface.begin();
}

void loop() {
    Control_Surface.loop();
}
```
Connections to bidirectional pipes are made using the “vertical pipe” operator
(`|`).

### Multiple MIDI interfaces {#midi_md-multiple-interfaces-routing}

If you only have a single MIDI interface, you don't really need to worry about
pipes, as you saw in the @ref midi_md-default-routing section, you can just
call `Control_Surface.begin()` and it will automatically set everything up for
you. 

MIDI pipes really shine in scenarios where you have multiple MIDI interfaces in 
a single program, they allow you to fully customize the routes between them. 
In the following example, we'll add an extra serial MIDI interface, and route 
it as follows:

<img style="margin: 0 auto; display: block; width: 38em" src="midi-pipes-2.svg"/>

```cpp
#include <Control_Surface.h>

// Instantiate a MIDI over USB interface
USBMIDI_Interface midi_usb;
// Instantiate a MIDI over Serial interface using `Serial1`
HardwareSerialMIDI_Interface midi_ser = Serial1;
// Instantiate the three pipes to connect the interfaces to Control_Surface
MIDI_Pipe pipe_tx_u, pipe_rx_u, pipe_tx_s;

NoteButton button {2, MIDI_Notes::C[4]};
NoteLED led {13, MIDI_Notes::C[4]};

void setup() {
    // Manually route MIDI output from Control_Surface to the USB MIDI interface
    Control_Surface >> pipe_tx_u >> midi_usb;
    // Manually route MIDI output from the USB MIDI interface to Control_Surface
    Control_Surface << pipe_rx_u << midi_usb;
    // Manually route MIDI output from Control_Surface to the Serial MIDI interface
    Control_Surface >> pipe_tx_s >> midi_ser;
    // Initialize everything
    Control_Surface.begin();
}

void loop() {
    Control_Surface.loop();
}
```

### MIDI pipe factories {#midi_md-pipe-factories}

If you want to keep adding new connections, you need a new pipe for each one.
For large numbers of pipes this is cumbersome, so the library includes a pipe
factory that gives you a new pipe every time you use it.

Using the factory, the previous example can be simplified as follows:

```cpp
#include <Control_Surface.h>

// Instantiate a MIDI over USB interface
USBMIDI_Interface midi_usb;
// Instantiate a MIDI over Serial interface using `Serial1`
HardwareSerialMIDI_Interface midi_ser = Serial1;
// Instantiate a factory that can produce three pipes
MIDI_PipeFactory<3> pipes;

NoteButton button {2, MIDI_Notes::C[4]};
NoteLED led {13, MIDI_Notes::C[4]};

void setup() {
    // Manually route MIDI output from Control_Surface to the USB MIDI interface
    Control_Surface >> pipes >> midi_usb;
    // Manually route MIDI output from the USB MIDI interface to Control_Surface
    Control_Surface << pipes << midi_usb;
    // Manually route MIDI output from Control_Surface to the Serial MIDI interface
    Control_Surface >> pipes >> midi_ser;
    // Initialize everything
    Control_Surface.begin();
}

void loop() {
    Control_Surface.loop();
}
```

The number of pipes that the factory can produce is specified between angle
brackets (`<>`). If you try to use more pipes than specified, you will get an
error at run time, which is indicated by a blinking on-board LED.

Instead of specifying a specific pipe when doing the routing in the `setup`
function, you can now just use the `pipes` factory, it will give you a new pipe
each time you use it.

If you need bidirectional pipes, you can use the 
@ref BidirectionalMIDI_PipeFactory class instead.

### Routing between MIDI interfaces {#midi_md-routing-between-interfaces}

Up to now, we've only created connections between `Control_Surface` itself and
a MIDI interface, but you can also create routes between MIDI interfaces. 
This is demonstrated in the following example, where we connect a debug MIDI
interface to a MIDI over USB interface.

<img style="margin: 0 auto; display: block; width: 38em" src="midi-pipes-interfaces.svg"/>

In this case, all messages that arrive on the `midi_dbg` interface are sent to 
the `midi_usb` interface, and all messages that arrive on `midi_usb` are sent to
`midi_dbg`.

Now that we're no longer using `Control_Surface.begin()` and 
`Control_Surface.loop()`, we have to take care of initializing and updating the 
MIDI interfaces ourselves.

```cpp
#include <Control_Surface.h>

// Instantiate a MIDI over USB interface
USBMIDI_Interface midi_usb;
// Instantiate a debug MIDI interface for debugging in the Serial Monitor
USBDebugMIDI_Interface midi_dbg;
// Instantiate the pipe to connect the two interfaces
BidirectionalMIDI_Pipe pipes;

void setup() {
    // Manually route MIDI input from the debug interface to the USB interface,
    // and the MIDI input from the USB interface to the debug interface
    midi_dbg | pipes | midi_usb;
    // Initialize the MIDI interfaces
    MIDI_Interface::beginAll();
}

void loop() {
    // Continuously poll all interfaces and route the traffic between them
    MIDI_Interface::updateAll();
}
```

If you have an Arduino with native MIDI over USB support, you can try out this
example yourself:  
Open your favorite MIDI monitor and select the MIDI over USB port of the Arduino.
Then open the Serial Monitor, set the line ending to “Newline”, and then enter a
MIDI message, e.g. `90 3C 7F`. You should see the message arrive in the MIDI
monitor as a Note On message for note C4 on channel 1 with a velocity of 127. 
Similarly, if you send a message to the Arduino using the MIDI monitor, it will
be printed in the Serial Monitor, because we used a bidirectional pipe.

If you only want to print MIDI messages, you can use the 
@ref USBDebugMIDI_Output class instead of @ref USBDebugMIDI_Interface. 
USBDebugMIDI_Output uses less memory, and one of the issues of having multiple
USBDebugMIDI_Interfaces is that input cannot work (some of the characters you 
type in are sent to one of the interfaces, some to the others), 
@ref USBDebugMIDI_Output doesn't have this issue.

In the following example, Control Surface works normally, using the USB MIDI
interface: you can try sending note on/off messages for note C4 and it'll turn
on/off the on-board LED, and if you press/release the button on pin 2 you'll 
receive note on/off messages.  
Additionally, all MIDI input that arrives on the MIDI over USB interface is 
routed to a secondary MIDI debug output, allowing you to use the Serial Monitor
to inspect the messages being sent to the Arduino.

<img style="margin: 0 auto; display: block; width: 38em" src="midi-pipes-mon.svg"/>

```cpp
#include <Control_Surface.h>

// The primary MIDI interface used by Control Surface.
USBMIDI_Interface midi;
// A secondary interface to print incoming messages
// on the primary interface to the serial monitor.
USBDebugMIDI_Output midimon { 115200 };
// Pipe to route messages from midi to midimon.
MIDI_Pipe mpipe;

// You can add normal Control Surface MIDI elements:
NoteButton btn {  2, MIDI_Notes::C[4] };
NoteLED    led { 13, MIDI_Notes::C[4] };

void setup() {
    midi.setAsDefault(); // Make this the primary interface.
    midi >> mpipe >> midimon; // Route messages from midi to midimon.
    Control_Surface.begin();
}

void loop() {
    Control_Surface.loop();
}
```

### Filtering or modifying messages passing through a pipe {#midi_md-pipes-filtering}

It is possible to create custom MIDI pipes that can filter out certain MIDI
messages, or even modify the MIDI messages that pass through them (for instance,
to change the channel of certain messages). This is done by inheriting from
the @ref MIDI_Pipe class and overriding the @ref MIDI_Pipe::mapForwardMIDI
functions. See @ref MIDI_Pipes-Filter.ino for more details.

### More examples {#midi_md-routing-more-examples}

- @ref MIDI_Pipes-Routing.ino
- @ref Dual-MIDI-Interface.ino
- @ref MIDI-Monitor.ino
- @ref MIDI_Pipes-Filter.ino
- @ref USBMIDI-Adapter.ino
- @ref BLEMIDI-Adapter.ino
