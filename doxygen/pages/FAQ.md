# Frequently Asked Questions

[TOC]

## Can I use multiplexers for reading rotary encoders? {#mux-encoder}

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
ESP32, etc.), you can still use polling, given that the rest of your code 
doesn't take too long.

There are two examples for using rotary encoders with a MCP23017:
 - @ref MCP23017-RotaryEncoder-Interrupts.ino (Teensy)
 - @ref MCP23017-RotaryEncoder-No-Interrupts.ino (Other boards)

## Can I change the address, controller number, MIDI channel, etc. at runtime? {#change-address-runtime}

Yes, this is done through banks. See the @ref Bank.ino example.  
The standard @ref BankableMIDIOutputElements have fixed offsets between the 
different addresses (e.g. address 8, 16, 24 and 32), while the 
@ref ManyAddressesMIDIOutputElements allow you to specify arbitrary addresses, 
they don't need to have a fixed offset between them.
