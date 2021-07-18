## TODO

- ✔ ~~Change template structure for senders~~
- More examples with better structure
- ✔ ~~Move Wikis to tttapa.github.io~~
- ✔ ~~Add FAQ page~~
- Add Troubleshooting page
- ✔ ~~Add a getter for the button state, and rename `getState()` → update()~~
- Cleanup structure of display elements.
- Flashing LEDs
- ✔ ~~Motorized faders~~
- Keywords
- Make many protected functions private if 1) they are not needed in tests 2) 
  they should not be called from within subclasses.
- ✔ ~~Make it easier to add custom bitmaps~~
- ✔ ~~Cleanup structure of input elements~~
- ✔ ~~Extension consistency~~
- ✔ ~~Malloc → new + smart pointers~~
- Make constructors of abstract classes protected.
- ✔ ~~Make destructors virtual if necessary. (e.g. callback classes)~~
- ✔ ~~GitHub template~~
- ✔ ~~Fix firmware link DFU scripts~~
- ✔ ~~Move DFU scripts → scripts~~
- List all TODO's throughout the source code and solve them
- ✔ ~~Full MIDI system common support (BLE)~~
- ✔ ~~Fix MCU LCD length~~
- ✔ ~~Return type transposers~~
- ✔ ~~Selection transposers~~
- optimized ExtIO for boards with tons of RAM?
- ✔ ~~Output only MIDI debug interface~~
- Bankable FastLED

## Specific elements

### Outputs

#### MIDI Notes
- ✔ Button
- ✔ Latching Button
- ✔ Buttons
- ✔ Button Matrix

- ✔ Bankable Button
- ✔ Bankable Latching Button
- ✔ Bankable Buttons
- ✔ Bankable Button Matrix

#### MIDI CC
- ✔ Button
- ✔ Latching Button
- ✔ Buttons
- ✔ Button Matrix

- ✔ Potentiometer
- Potentiometers

- ✔ Rotary Encoder

- ✔ Bankable Button
- ✔ Bankable Latching Button
- ✔ Bankable Buttons
- ✔ Bankable Button Matrix

- ✔ Bankable Potentiometer
- Bankable Potentiometers

- ✔ Bankable Rotary Encoder

#### MIDI Pitch Bend
- ✔ Potentiometer
- Potentiometers
- Motorized fader

- ✔ Bankable Potentiometer
- Bankable Potentiometers
- Bankable Motorized fader


### Inputs

#### Generic
- ✔ LED

- ✔ Bankable LED

#### MCU
- ✔ VPotRing
- ✔ VU
- ✔ Assignment display
- ✔ Time display
- ✔ LCD display (SysEx)
- Flashing LED

- ✔ Bankable VPotRing
- ✔ Bankable VU
- Flashing LED


### Display
- ✔ Selectors
- ✔ VPotRing
- ✔ VU
- ✔ LED Bitmap
- ✔ Timedisplay
- ✔ Assignment display
- ✔ LCD


### Selectors
- ✔ Switch
- Latching
- ✔ Encoder
- ✔ Increment
- ✔ Increment/Decrement
- ✔ Many Buttons

### Selectable
- ✔ Bank
- ✔ Program Change
- ✔ Transpose
