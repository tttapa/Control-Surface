# Using the MIDI controller library
## 1. Add the library to your sketch
First add the library to your sketch by simply typing this line at the top of your file:  
`#include <MIDI_Controller.h>`

## 2. Add a MIDI interface (optional)
If you don't explicitly instantiate a MIDI interface, the MIDI Controller library will
instantiate a MIDI interface that sends MIDI messages over the USB connection (`USBMIDI_Interface`).

If you do explicitly instantiate a MIDI interface,
it will be used as MIDI output by the MIDI Controller library.  

If you instantiate more than one MIDI interfaces, the interface that's the last
one to be instantiated will be the default output, unless you specify a default interface using  
`MIDI_Interface::setDefault();`  

There are many MIDI interfaces to choose from:  
`USBMIDI_Interface`
`StreamMIDI_Interface(Stream &stream)`  
`SerialMIDI_Interface(Serial_t &serial, unsigned long baud)`
`HardwareSerialMIDI_Interface(HardwareSerial &serial, unsigned long baud)`
`USBSerialMIDI_Interface(unsigned long baud)`
`HairlessMIDI_Interface`
`SoftwarSerialMIDI_Interface(SoftwareSerial &serial, unsigned long baud)`

There's also a MIDI debug mode, that can be used on these same serial interfaces:  
`StreamDebugMIDI_Interface(Stream &stream)`
`SerialDebugMIDI_Interface(Serial_t &serial, unsigned long baud)`
`HardwareSerialDebugMIDI_Interface(HardwareSerial &serial, unsigned long baud)`
`USBDebugMIDI_Interface(unsigned long baud)`
`SoftwarSerialDebugMIDI_Interface(SoftwareSerial &serial, unsigned long baud)`

## 3. Add Extended IO elements (optional)

## 4. Add MIDI Control Elements
Next, add some MIDI control elements, like buttons, potentiometers, rotarty encoders ...
Instantiate MIDI control element objects from the classes `Analog`, `AnalogHiRes`, `Digital`, `DigitalLatch` and `RotaryEncoder`.

### Analog
`Analog(pin_t analogPin, uint8_t controllerNumber, uint8_t channel)`  
Analog control elements send the value of an analog input as a 7-bit MIDI Control Change event.  
It can be used for potentiometers, linear faders, analog sensors ...
and can be mapped to controls like volume, balance, effect parameters, EQ ...  
`analogPin`: the analog input pin to read the value from  
`controllerNumber`:  the [MIDI Control Change controller number](https://www.midi.org/specifications/item/table-3-control-change-messages-data-bytes-2) [0, 119]  
`channel`: the MIDI channel [1, 16]  
The analog input value will be averaged over 8 samples.

### AnalogHiRes
`AnalogHiRes(pin_t analogPin, uint8_t channel)`  
AnalogHiRes control elements send the value of an analog input as a 14-bit MIDI Pitch Bend event.  
It can be used for potentiometers, linear faders, analog sensors ...
and can be mapped to controls like volume, balance, effect parameters, EQ ...  
`analogPin`: the analog input pin to read the value from  
`channel`: the MIDI channel [1, 16]  
The analog input value will be averaged over 16 samples.
The actual resolution is only 10 bits. The 10-bit value is padded with 4 zeros in order to fit in a 14-bit Pitch Bend event.

### Digital
`Digital(pin_t pin, uint8_t note, uint8_t channel, uint8_t velocity = 127)`  
Digital control elements send the state of a push button as MIDI Note On and Note Off events.  
When the button is pushed, a Note On event is sent, when it is released, a Note Off event is sent.
It can be used for momentary push buttons and other momentary digital inputs.
It can be mapped to controls like transport control (play/pause/stop/cue/... buttons), mute/solo/rec buttons, effect enable/disable, looping options, sample triggers ...  
`pin`: the digital input pin to read the state from, the internal pull-up resistor will be enabled  
`note`: the [MIDI note number](http://www.electronics.dit.ie/staff/tscarff/Music_technology/midi/midi_note_numbers_for_octaves.htm) [0, 127]  
`channel`: the MIDI channel [1, 16]  
`velocity`: the MIDI velocity of the Note events [1, 127], how hard the key/button is struck  
The button is debounced in software. The default debounce time is 25 ms.

### DigitalLatch
`DigitalLatch(pin_t pin, uint8_t note, uint8_t channel, uint8_t velocity, unsigned long latchTime)`  
DigitalLatch control elements are similar to Digital elements, but they send a MIDI Note On and a Note Off event every time the input state changes. This allows you to use toggle switches or other types of latching switches.
When the switch is turned on, a Note On event is sent, then, some time later, a Note Off event is sent. When the switch is turned off, the same happens (a Note On event is sent, then, some time later, a Note Off event is sent).  
It can be mapped to controls like mute/solo/rec buttons, effect enable/disable, looping options ...  
`pin`: the digital input pin to read the state from, the internal pull-up resistor will be enabled  
`note`: the [MIDI note number](http://www.electronics.dit.ie/staff/tscarff/Music_technology/midi/midi_note_numbers_for_octaves.htm) [0, 127]  
`channel`: the MIDI channel [1, 16]  
`velocity`: the MIDI velocity of the Note events [1, 127], how hard the key/button is struck  
`latchTime`: the time between the Note On and the Note Off events  
The switch is not debounced in software.

### RotaryEncoder
`RotaryEncoder(uint8_t pinA, uint8_t pinB, uint8_t controllerNumber, uint8_t channel, int speedMultiply = 1, uint8_t pulsesPerStep = NORMAL_ENCODER, relativeCCmode mode = TWOS_COMPLEMENT)`  
RotaryEncoder control elements send the relative change of the position of a quadrature encoder using MIDI Control Change events.  
`pinA`: the first pin of the encoder's quadrature output, the internal pull-up resistor will be enabled  
`pinB`: the second pin of the encoder's quadrature output, the internal pull-up resistor will be enabled  
`controllerNumber`:  the [MIDI Control Change controller number](https://www.midi.org/specifications/item/table-3-control-change-messages-data-bytes-2) [0, 119]  
`channel`: the MIDI channel [1, 16]  
`speedMultiply`: a factor that is multiplied with the position change to make the control move faster, default is 1  
`pulsesPerStep`: a factor that the position change is divided by to make the control move slower and to have finer control: in the case of a normal rotary encoder, there are 4 pulses between two detents, so setting the number of pulses per step to 4 (using the constant `NORMAL_ENCODER`) ensures that moving the encoder one click results in a value change of one, instead of four,  default is 4, for a jog wheel (without detents), use the constant `JOG`  
`mode`: the mode to encode negative position changes: MIDI Control Change events have a 7-bit unsigned integer as value byte. To represent negative numbers, you can either use 7-bit two's complement (`TWOS_COMPLEMENT`), 7-bit signed magnitude (`SIGN_MAGNITUDE`), or add a constant offset of 64 to the value (`BINARY_OFFSET`), default is `TWOS_COMPLEMENT`  
You can use one of the following aliases as well: `REAPER_RELATIVE_1`, `REAPER_RELATIVE_2`, `REAPER_RELATIVE_3`, `TRACKTION_RELATIVE`, `MACKIE_CONTROL_RELATIVE`  

## 5. Add Banks (optional)
`Bank(uint8_t channelsPerBank = 1);`  
Using banks, you can group control elements together, and it allows you to change the MIDI channel and controller or note numbers of these elements.  
For example, if you have four volume control sliders for tracks 1, 2, 3 and 4, you can add them to a bank. If you set te bank to setting 0, the sliders will control the volume of tracks 1, 2, 3 and 4, if you set the bank to setting 1, the sliders will instead control the volume of tracks 5, 6, 7 and 8, set it to 2, and it will affect tracks 9, 10, 11 and 12, etc.  
This allows you to control a large number of tracks or controls, with only a limited number of physical knobs and buttons.  
`channelsPerBank`: the number of channels/tracks each bank can control at one instant, e.g. in the example above, there are four sliders, so they can control four tracks at once  

## 6. Add Bank Selectors (optional)
You could select the setting of a bank manually, using  
`Bank::setBankSetting(uint8_t bankSetting);`  
`bankSetting`: the bank to select (zero-based)  

However, it's much easier to use a BankSelector object to set the bank setting for you. A Bank Selector takes input from buttons or switches, updates the bank setting accordingly, and can provide visual feedback using LEDs.  
There are many different bank selector modes, for example: one button to increment the bank setting, another to decrement it, or one button for each bank, with LEDs or without ...  
All possible modes are explained in [Appendix A](#appendix-a-bankselector-modes).

## 7. Add Control Elements to the Banks (optional)
You can add MIDI Control Elements to a Bank using  
`Bank::add(MIDI_Control_Element *element, bankType type = CHANGE_ADDRESS);`  
`Bank::add(MIDI_Control_Element &element, bankType type = CHANGE_ADDRESS);`  
`Bank::add(MIDI_Control_Element* arr[N], bankType type = CHANGE_ADDRESS);`  
`Bank::add(MIDI_Control_Element& arr[N], bankType type = CHANGE_ADDRESS);`  
`element`: either a pointer or a reference to a MIDI Control Element    
`arr`: an array of pointers or references to MIDI Control Elements  
`type`: determines the behavior of the bank: if it is set to `Bank::CHANGE_ADDRESS`, the bank setting will alter the address (note number or controller number) of the control element, if type is set to `Bank::CHANGE_CHANNEL`, the bank setting will alter the MIDI channel of the control element, default is `CHANGE_ADDRESS`  

## 8. Set analog map functions and invert buttons (optional)
There may be situations where you want more control over the analog input values before they are sent over MIDI. For example, if you use logarithmic taper potentiometers, you may want to map it to a linear curve first, or if the potentiometer reads 1010 in the maximum position, instead of 1023, you may want to calibrate it.  
You can write your own function that performs this mapping or calibration, and add it to an Analog or AnalogHiRes control element using  
`Analog::map(int (*fn)(int));` or `AnalogHiRes::map(int (*fn)(int));`  
`fn`: a function (or pointer to a function) that takes one integer argument, i.e. the raw analog value [0, 1023], and returns an integer, the mapped value [0, 1023]  
You can apply the same mapping function to all Analog and AnalogHiRes control elements in the same bank using  
`Bank::map(int (*fn)(int));`

When using normal buttons, they are connected between an input pin with the internal pull-up resistor enabled, and ground. This means that when they are pressed, the input reads low, and when they are released, the input reads high. There may be cases where you want the input to be high when the button is pressed, and low when it's released. To do this, you can use  
`Digital::invert();`
You can invert all Digital control elements in the same bank using  
`Bank::invert();`  

## 9. Refresh the MIDI Controller
In the loop, refresh the MIDI Controller using  
`MIDI_Controller.refresh();`  
If you are doing other things in the loop, make sure that they are non-blocking, or use a timer interrupt.


***


## Appendix A: BankSelector modes
  - One toggle switch (latching switch)  

      When the switch is in the 'off' position, bankSetting 1 is selected  
      When the switch is in the 'on' position, bankSetting 2 is selected  

      `BankSelector(bank, switch pin, BankSelector::TOGGLE);`    


  - One toggle switch (latching switch) and one LED  

      When the switch is in the 'off' position, bankSetting 1 is selected and the LED is off  
      When the switch is in the 'on' position, bankSetting 2 is selected and the LED is on  

      _Note: this mode is pretty useless, you can just connect the LED to the switch directly, without wasting a digital output pin on it._  

      `BankSelector(bank, switch pin, led pin, BankSelector::TOGGLE);`  


  - One momentary switch (push button)  

      Pressing the button switches the bankSetting:  
      When starting the program, bankSetting 1 is selected,  
      When the button is pressed, bankSetting 2 is selected,  
      When the button is pressed again, bankSetting 1 is selected,
      and so on.  

      `BankSelector(bank, button pin);`  
      `BankSelector(bank, button pin, BankSelector::MOMENTARY);`  


  - One momentary switch (push button) and one LED   

      Pressing the button switches the bankSetting and toggles the LED:  
      When starting the program, bankSetting 1 is selected and the LED is off,  
      When the button is pressed, bankSetting 2 is selected and the LED turns on,  
      When the button is pressed again, bankSetting 1 is selected and the LED is turned off,
      and so on.

      `BankSelector(bank, button pin, led pin);`  
      `BankSelector(bank, button pin, led pin, BankSelector::MOMENTARY);`  


  - Multiple momentary switches (push buttons)  

      Pressing one of the buttons selects the respective output:  
      When starting the program, bankSetting 1 is selected,  
      When the second button is pressed, bankSetting 2 is selected,  
      When the n-th button is pressed, bankSetting n is selected.  

      `BankSelector(bank, { button 1 pin, button 2 pin, ... , button n pin } );`  


  - Multiple momentary switches (push buttons) and multiple LEDs  

      Pressing one of the buttons selects the respective output and enables the respective LED:  
      When starting the program, bankSetting 1 is selected and LED 1 is on,  
      When the second button is pressed, bankSetting 2 is selected, LED 1 turns off and LED 2 turns on,  
      When the n-th button is pressed, bankSetting n is selected, LED n turns on, and all other LEDs are off.  

      `BankSelector(bank, { button 1 pin, button 2 pin, ... , button n pin }, { led 1 pin, led 2 pin, ... , led n pin } );`

      
  - Two momentary switches (push buttons)  

      Pressing the first button increments the bankSetting number,  
      pressing the second button decrements the bankSetting number:  
      When starting the program, bankSetting 1 is selected,  
      When the first button is pressed, bankSetting 2 is selected,  
      When the first button is pressed again, bankSetting 3 is selected,  
      When the last bankSetting is selected, and the first button is pressed again,
      bankSetting 1 is selected.  
      When the second button is pressed, the last bankSetting (n) is selected,  
      When the second button is pressed again, bankSetting (n-1) is selected,
      and so on.  

      `BankSelector(bank, { button increment pin, button decrement pin }, number of bankSettings);`  


  - Two momentary switches (push buttons) and multiple LEDs  

      Pressing the first button increments the bankSetting number and turns on the respective LED,  
      pressing the second button decrements the bankSetting number and turns on the respective LED:  
      When starting the program, bankSetting 1 is selected and LED 1 is on,  
      When the first button is pressed, bankSetting 2 is selected, LED 1 turns off and LED 2 turns on,  
      When the first button is pressed again, bankSetting 3 is selected, LED 2 turns off and LED 3 turns on.  
      When the last bankSetting is selected, and the first button is pressed,
      bankSetting 1 is selected, the last LED turns off and LED 1 turns on.  
      When the second button is pressed, the last bankSetting (n) is selected, LED 1 turns off and LED n turns on,  
      When the second button is pressed again, bankSetting (n-1) is selected, LED n turns off and LED n-1 turns on,
      and so on.  

      `BankSelector(bank, { button increment pin, button decrement pin }, { led 1 pin, led 2 pin, ... , led n pin } );`  


  - One momentary switch (push button)  

      Pressing the button increments the bankSetting number,  
      When starting the program, bankSetting 1 is selected,  
      When the button is pressed, bankSetting 2 is selected,  
      When the button is pressed again, bankSetting 3 is selected,  
      When the last bankSetting is selected, and the button is pressed again,
      bankSetting 1 is selected.  

      `BankSelector(bank, { button increment pin }, number of bankSettings);`  


  - One momentary switch (push button) and multiple LEDs  

      Pressing the button increments the bankSetting number and turns on the respective LED,  
      When starting the program, bankSetting 1 is selected and LED 1 is on,  
      When the button is pressed, bankSetting 2 is selected, LED 1 turns off and LED 2 turns on,  
      When the button is pressed again, bankSetting 3 is selected, LED 2 turns off and LED 3 turns on.  
      When the last bankSetting is selected, and the button is pressed,
      bankSetting 1 is selected, the last LED turns off and LED 1 turns on.  
      
      `BankSelector(bank, { button increment pin }, { led 1 pin, led 2 pin, ... , led n pin } );`  


_Note: a switch is 'off' or 'released' when it doesn't conduct. The digital value
on the input will therefore be HIGH (because of the pull-up resistor)_
