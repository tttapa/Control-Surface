# MIDI Tutorial {#midi-tutorial}

[TOC]

## The MIDI protocol

See <https://www.midi.org/>, specifically the 
[summary of MIDI messages](https://www.midi.org/specifications-old/item/table-1-summary-of-midi-message).

## MIDI interfaces {#midi_md-midi-interfaces}

Control Surface supports sending and receiving MIDI messages in many different 
ways: over a standard 5-pin DIN MIDI cable, over USB, over Bluetooth, over WiFi
...  
Each of those methods is available as a “MIDI Interface” class in the code, you 
can find a full overview in the @ref MIDIInterfaces module.

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

Other available interfaces are @ref BluetoothMIDI_Interface, 
@ref HairlessMIDI_Interface, @ref SoftwareSerialMIDI_Interface, 
@ref USBHostMIDI_Interface ...

### Supported Arduino-compatible boards {#midi_md-interfaces-supported-boards}

Not all MIDI interfaces are supported on all Arduino boards. For example, not 
all Arduino boards support MIDI over USB natively. You can find an overview 
of boards that do support it on the @ref md_pages_MIDI-over-USB page.

MIDI over BLE is currently only supported on the ESP32.  
USB Host MIDI is only supported on the Teensy 3.6 and 4.1 boards.

### Functionality {#midi_md-interfaces-functionality}

The MIDI interfaces provide the following functionality:
- **Sending MIDI**: You can send MIDI messages from your Arduino code to 
whatever device is connected on the other side of the interface.
- **Receiving MIDI**: MIDI messages sent by the device on the other side can 
be read and inspected in your code, or you can register a callback that gets 
called whenever a message arrives through the interface.
- **Routing MIDI**: %Interfaces can be set up such that MIDI messages that arrive
on one interface are automatically routed to other interfaces, and you can 
filter or modify the messages in between.

In the remainder of this tutorial, one section will be devoted to each of these
functions.

### Instantiating a MIDI interface {#midi_md-instantiate-a-midi-interface}

The following code snippet demonstrates how to instantiate, initialize, and 
update a MIDI over USB interface. By “instantiate”, we mean that a variable is 
created that is an _instance_ of the `USBMIDI_Interface` type.

```cpp
#include <Control_Surface.hpp>

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
snippet are shown in blue: these are links to the documentation for the relevant
classes and functions. If you hover over them with your mouse pointer, you'll 
see a short description, and if you click on the link, it will take you to the 
detailed documentation.

The code above doesn't really anything useful yet, it just sets up everything
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
const MIDIAddress note = MIDI_Notes::C(4); // C(4) is middle C
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
The function `MIDI_Notes::C(4)` gives you the note C in the fourth octave, or 
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
the blue links in the code snippet.

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
            Channel channel = CHANNEL_1,
            Cable cableNumber = CABLE_1)
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
MIDIAddress myAddress = {42, CHANNEL_2};

// Address 111 on channel 2 and cable 3:
MIDIAddress myAddress = {111, CHANNEL_2, CABLE_3};
~~~

If you specify just a single argument, as in the first example, you don't have
to use curly braces (`{}`). If you specify more than one argument, you need 
curly braces.

#### MIDI Note names {#midi_md-midi-note-names}

For entering MIDI note numbers, you can use the note names in the 
@ref MIDI_Notes namespace:
~~~cpp
// MIDI Note middle C (C in the fourth octave) on MIDI Channel 16:
MIDIAddress myAddress = {MIDI_Notes::C(4), CHANNEL_16};
~~~

For reasons explained [here](@ref faq-midi-note-f), the note F is an exception,
you have to add a trailing underscore to avoid conflicts with Arduino's `F(...)`
macro:
~~~cpp
// MIDI Note F in the fourth octave on MIDI Channel 16:
MIDIAddress myAddress = {MIDI_Notes::F_(4), CHANNEL_16};
//          note the underscore here  ^
~~~
If you look at the full list of note names in the @ref MIDI_Notes namespace, 
you'll see that the sharps are missing: you can simply use the flats instead 
(e.g. instead of using G♯, you can use A♭).

#### MIDI Control Change names {#midi_md-midi-control-change-names}

Instead of specifying the controller number (address) as a number, you can also
use the predefined constants in the @ref MIDI_CC namespace:
~~~cpp
// Control Change General Purpose Controller #1 on MIDI Channel 2:
MIDIAddress myAddress = {MIDI_CC::General_Purpose_Controller_1, CHANNEL_2};
~~~

#### MIDI Program Change names {#midi_md-midi-program-change-names}

Similar to the constants for controller numbers, you can find the constants for 
all General MIDI program numbers in the @ref MIDI_PC namespace:
~~~cpp
// Harpsichord voice on MIDI Channel 9.
MIDIAddress myAddress = {MIDI_PC::Harpsichord, CHANNEL_9};
~~~

### More examples {#midi_md-sending-more-examples}

- @ref MIDI-Output.ino
- @ref Send-MIDI-Notes.ino
- @ref SysEx-Send-Receive.ino

## Receiving MIDI messages {#midi_md-receiving}

MIDI input is usually handled using callback functions. First, an example:

### Simple example {#midi_md-receiving-simple-example}

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
    midi.update(); // Continuously handle MIDI input
}
```

The instantiation, initialization, and updating of the MIDI interface is the 
same as before. The first new component is the `MyMIDI_Callbacks` class: this 
class inherits from the @ref MIDI_Callbacks class that is provided by the 
library, and then implements one of the callback functions, 
@ref MIDI_Callbacks::onChannelMessage().  
An instance of the `MyMIDI_Callbacks` class is created, and then this callback 
is attached to the MIDI interface in the `setup` function using 
@ref MIDI_Interface::setCallbacks().

After the callback is attached, every time you call `midi.update()` in the 
`loop`, the MIDI interface will check if any new messages have arrived, and if
that's the case, it will call the corresponding callback function. For example,
whenever a MIDI channel message (such as Note On/Off, Control Change, etc.)
arrives, the `onChannelMessage()` function of the callback is called.

The first argument of the callback function is a reference to the MIDI interface
that received the message (this is not used in this simple example), and the 
second argument is the message itself.

### Accessing the contents of the message {#midi_md-accessing-the-contents-of-the-message}

The previous example simply printed “Channel message received” whenever it 
received a channel message. In most cases, this is not really that useful, so we
can improve upon it by inspecting the contents of the messages.

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
then prints them to the serial port in hexadecimal.

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

If you send a Note On message on MIDI Channel 4 for 
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

You can even go further, for example, matching only a specific note on a 
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
        bool our_note = channel == CHANNEL_1 && note == MIDI_Notes::C(4);
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
        const MIDIAddress our_special_note = {MIDI_Notes::C(4), CHANNEL_1};
        // If it's a note message that matches our specific address, print it
        if (note_msg && msg.getAddress() == our_special_note)
            Serial << type << " middle C on Channel 1 with velocity "
                   << velocity << endl;
    }
 
} callback; // Instantiate a callback
```

### Fine-grained callbacks {#midi_md-fine-grained-callbacks}

The callbacks we wrote in the previous section are very flexible and allow you 
to access all fields of the MIDI message and the MIDI interface. In some cases
though, that's overkill, and you might want to avoid the boilerplate of having
to check the message type each time.

In that case, you can use the @ref FineGrainedMIDI_Callbacks class, which allows
you to write a callback for each specific MIDI message type. For example:
```cpp
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
```
This approach is used in the @ref MIDI-Input-Fine-Grained.ino example. You can
find the full list of available callbacks and their arguments in the 
documentation for @ref FineGrainedMIDI_Callbacks, under 
@ref FineGrainedMIDI_Callbacks_MIDI_Callback_Functions "MIDI Callback Functions",
and in the @ref MIDI-Input-Fine-Grained-All-Callbacks.ino example.

### More examples {#midi_md-receiving-more-examples}

- @ref MIDI-Input.ino
- @ref MIDI-Input-Fine-Grained.ino
- @ref MIDI-Input-Fine-Grained-All-Callbacks.ino
- @ref SysEx-Send-Receive.ino

## Routing MIDI messages {#midi_md-routing}

==TODO==

### More examples {#midi_md-routing-more-examples}

- @ref MIDI_Pipes-Routing.ino
