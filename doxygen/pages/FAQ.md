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
