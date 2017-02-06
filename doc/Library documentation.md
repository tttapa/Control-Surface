# Using the MIDI controller library
First add the library to your sketch by simply typing this line at the top of your file:  
`#include <MIDI_controller.h>`

The library currently has 4 classes: Analog, Digital, DigitalLatch, and Encoder.

Each class has a set of functions, and a set of variables. Some are private, and are used inside the class, others are public, and can be called from outside of the class. More on classes [here](https://www.arduino.cc/en/Hacking/LibraryTutorial).

The first step in using a class is creating an instance of the class. This is mostly done before the setup, so you can use the instance everywhere in the program.

For example, let's add an instance 'potentiometer' of the class Analog:  
`Analog potentiometer(foo, bar, ... );` (Just ignore the foo-bar for now)

Now that the instance is created, we can use the functions of Analog on it, for example, the refresh function:  
`potentiometer.refresh();`  

For explanation of what the functions do and what they are made for, read following reference.

# Library Reference

## Analog
The analog class is meant for analog inputs, like potentiometers and faders (or analog sensors that output a value between 0 and 5V). 

### Creating an instance

`Analog (byte pin, byte controller, byte channel, byte resolution);`

**pin** is the analog pin to read from. It's connected to the wiper of the potentiometer.

**controller** is the MIDI controller number (data 1 in the MIDI message). This is how it will appear in your MIDI software, like a unique address.

**channel** is the MIDI channel (1-15).

**resolution** is the resolution of the analog reading. A resolution of 2 would give only 2 possible values (0 and 127), a resolution of 3 would give 3 possible values (0, 63 and 127). A resolution of 128 gives 128 possible values (0-127), this is the default, because it's all 7-bit numbers. (MIDI uses 7-bit numbers for sending values) Use a value lower than 128, like 64, if there's a lot of noise on your input, for example if the controller keeps on sending 0,1,0,1,0,1,0,1, ... Use 128 if you want to use the maximum available resolution.

### Functions

`refresh();`

This function checks the input, and if it has changed since last time refresh was called, it sends the new value over MIDI (on the predefined controller number and channel).

`bank(byte pin, byte controller, byte channel);`

This function enables you to use one analog input (together with a switch) for multiple controls. If the switch is in the OFF position, the controller and channel will be those that were defined during instance creation, if the switch is in the ON position, the controller and channel will be those that were entered as arguments of this function.

**pin** is the digital pin with the switch connected. The internal pull-up resistor will be enabled.

**controller** is the controller to use when the switch is on.

**channel** is the channel to use when the switch is on.

`detachBank();`

This function disables the bank functionality that was set up with the bank function. The controller and channel will be ones that were defined during instance creation, regardless of the state of the switch.
The pin of the switch defined in the bank function will be set as an input without pull-up resistor again.

### Constants

None.

### Examples

Analog\_example  
Analog\_bank\_example

## Digital
The Digital class is meant for use with momentary pushbuttons. It sends a noteOn message when the button is pressed, and a noteOff message when the button is released. Connect your buttons between the input pin and the ground, the internal pull-up resistors will be used.

### Creating an instance

`Digital (byte pin, byte note, byte channel, byte velocity);`

**pin** is the digital pin with the button connected.

**note** is the MIDI note to send. 60 is middle C, and every half note is plus or minus one. You can compare this to the controller number in Analog.

**channel** is the MIDI channel.

**velocity** is how hard the note is hit. This doesn't matter for a normal button, as long as it's not 0. (It does matter however for touch sensitive pads for example.)

### Functions

`refresh();`

This function checks the input, and if it has changed since last time refresh was called, it sends the appropriate message: noteOn when the button is pressed, noteOff when the button is released.

`bank(byte pin, byte note, byte channel);`

This function enables you to use one button (together with a switch) for multiple controls. If the switch is in the OFF position, the note and channel will be those that were defined during instance creation, if the switch is in the ON position, the note and channel will be those that were entered as arguments of this function.

**pin** is the digital pin with the switch connected. The internal pull-up resistor will be enabled.

**note** is the note to use when the switch is on.

**channel** is the channel to use when the switch is on.

`detachBank();`

This function disables the bank functionality that was set up with the bank function. The note and channel will be the ones that were defined during instance creation, regardless of the state of the switch. The pin of the switch defined in the bank function will be set as an input without pull-up resistor again.

### Variables

`bool invert`

False by default. If you set it to `true`, a digital high (switch open) will send a note on message, and a digital low (switch closed) will send a note off message.

### Constants

None.

### Examples

Digital\_example  
Digital\_bank\_example  
Digital\_example\_invert

## DigitalLatch
The DigitalLatch class is meant to be used with toggle switches, that are not momentary. It sends both a noteOn and a noteOff message, every time the state changes. The delay between the on and off message can be set. If the state changes before the delay has finished, the noteOff message is sent anyway, and another 'pulse' is started: another noteOn message is sent, and after the set delay, the noteOff message is sent as well.
This class is useful if you want to use the 'mute' or 'solo' buttons in your software with non-momentary toggle switches. If you switch on the switch, the appropriate track is muted, when you switch the switch off again, the track will be un-muted, for example. 
Connect your switches between the input pin and the ground, the internal pull-up resistors will be used. 

### Creating an instance

`DigitalLatch (byte pin, byte note, byte channel, byte velocity, int delay);`

**pin** is the digital pin with the switch connected. The internal pull-up resistor will be enabled.

**note** is the MIDI note to send. 60 is middle C, and every half note is plus or minus one. You can compare this to the controller number in Analog.

**channel** is the MIDI channel.

**velocity** is how hard the note is hit. This doesn't matter for a normal switch, as long as it's not 0.

**delay** is the delay between the noteOn and noteOff messages, in milliseconds. 100ms works great.

### Functions

`refresh();`

This function checks the input, and it sends the appropriate message, as explained in the description off this class. (Note: the noteOff message is not sent, if this function isn't called again, after sending the noteOn message. If you just put this in your loop, however, you should be fine, but don't use `delay()` or other blocking functions.)

`bank(byte pin, byte note, byte channel);`

This function enables you to use one switch (together with a bank switch) for multiple controls. If the bank switch is in the OFF position, the note and channel will be those that were defined during instance creation, if the bank switch is in the ON position, the note and channel will be those that were entered as arguments of this function.

**pin** is the digital pin with the switch connected. The internal pull-up resistor will be enabled.

**note** is the note to use when the switch is on.

**channel** is the channel to use when the switch is on.

`detachBank();`

This function disables the bank functionality that was set up with the bank function. The note and channel will be the ones that were defined during instance creation, regardless of the state of the switch. The pin of the switch defined in the bank function will be set as an input without pull-up resistor again.

### Constants

None.

### Examples

DigitalLatch_example  
DigitalLatch_bank_example

## RotaryEncoder
This library is meant to use with a quadrature encoder. It requires [PJRC's Encoder library](https://www.pjrc.com/teensy/td_libs_Encoder.html)  to be installed.  
It sends relative messages. The way negative values are handled can be changed, because it depends on the software you're using.  
Connect the common pin of the encoder to the ground, the internal pull-up resistors will be used.

### Creating an instance

`RotaryEncoder(byte pin1, byte pin2, byte controller, byte channel, int speedMultiply, byte pulsesPerStep, byte mode);`

**pin1** is the first interrupt pin with the encoder connected.

**pin2** is the second interrupt pin with the encoder connected.

**controller** is the MIDI controller number (data 1 in the MIDI message). This is how it will appear in your MIDI software, like a unique address.

**channel** is the MIDI channel.

**speedMultiply** is the value that will be multiplied with the relative displacement, if the encoder is not fast enough in your software. If, for example, speedMultiply is set to 5, and the encoder were to send a '+1' message, a '+5' message will now be sent. Default is 1.

**pulsesPerStep** is the number of pulses the encoder outputs when you turn it one step or click. On a normal rotary encoder, this is 4. When you set it to 4, it will change 1 unit in your software per click you turn, instead of 4. This is mostly more logical. For jog wheels however, you may want to set it to 1, to take advantage of the full resolution of the wheel. Use 'NORMAL_ENCODER' or 'JOG' as argument.

**mode** is the way the MIDI message is sent in order to make it a signed number. There are 3 modes available. Use 'ADD_64', 'SIGN_BIT' or 'POS1_NEG127' as argument.

See the 'Constants' section for more information.

### Functions

`refresh();`

This function checks the encoder position, and if it has changed since last time, the relative change is sent over MIDI.

### Constants

`NORMAL_ENCODER`  
set pulsesPerStep to 4, for normal rotary encoders.

`JOG`  
set pulsesPerStep to 1, for jog wheels.

`ADD_64`  
First mode for relative MIDI messages. This is probably the simplest one. This basically maps 0 to 64 (which is 128/2). For example, if I want to send -1, I add 64, = 63 and I send it. If I want to send +1, I also add 64, = 65. If I just send 64, the computer will do nothing, because it knows it's a displacement of 0. (On the computer side, they just subtract 64, and you can use the result like nothing ever happened.)

`SIGN_BIT`  
Second mode for relative MIDI messages. On computers, signed values are mostly saved with a sign bit. The sign bit is the most significant bit. When it's 0, the number defined by the other bits is positive, when it's 1, the number is negative. In a MIDI message, this is bit 6 (the 7th bit, since it's 0 based). For example: +4 would be 0b00000100, and -4 would be 0b01000100.

`POS1_NEG127`  
Third mode for relative MIDI messages. Define +1 as 1, and -1 as 127. We can continue this: +2 = 2, and -2 = 126, etc. until +63 = 63, and -63 = 65.

### Examples

Encoder_example

## USBMidi
This is the class for MIDI communication.
It automatically finds out what Arduino you are using, and chooses the right settings to send a MIDI message over USB. 

### Creating an instance

You don't have to create one, you can just use the readily available `USBMidiController` object.

### Functions

`send(byte message, byte channel, byte data1, byte data2);`

Sends a 3-byte MIDI message over USB.

**message** is the first nibble of the message: the MIDI voice message (e.g. 0x90 for note on)

**channel** is the MIDI channel, a number from 1 to 16.

**data1** is the first data byte (7-bits)

**data2** is the second data byte (7-bits)

`send(byte message, byte channel, byte data);`

Sends a 2-byte MIDI message over USB.

**message** is the first nibble of the message: the MIDI voice message (e.g. 0xC0 for program change)

**channel** is the MIDI channel, a number from 1 to 16.

**data** is the data byte (7-bits)

`setDelay(int delay);`

Sets the delay between two messages, to prevent an overflow of data on the connection, and to get a nice blinking effect of the LED. 10-20ms works fine.

**delay** is the delay in milliseconds.

`blink(byte pin)`

Blinks the LED on every MIDI message. Helps for debuggin, and is aesthetically pleasing.

**pin** is the digital pin with the LED connected (pin 13 on most boards).

`noBlink();`

Disables the blinking effect. You don't need to call this function explicitly, the blinking is disabled by default.

### Constants

`NOTE_ON`  
Message type: 0x90, use to turn on a note. Use NOTE_OFF or send again with velocity = 0 to turn off again.

`NOTE_OFF`  
Message type: 0x80, use to turn off a note.

`CC`  
Message type: 0xB0 (Control Change), use to send analog values, like values from a potentiometers.

`PROGRAM_CHANGE`  
Message type: 0xC0 (Program Change), use to set the instrument of a certain channel. (Channel 10 is percussion).

`PITCH_BEND`  
Message type: 0xE0 (Pitch Bend), use to send large analog values.

### Examples

sendMidi_example_intoxicated