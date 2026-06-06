# Examples {#examples}

@tableofcontents

## Getting started {#examples-getting-started}
 - @ref 1.First-Output.ino
   (Multiplexed potentiometers that send MIDI Control Change messages)
 - @ref 2.First-Input.ino
   (LEDs driven by a shift register that turn on/off when a MIDI Note On/Off message is received)

## MIDI interfaces {#examples-midi-interfaces}
### Interfaces, transports and routing {#examples-midi-interfaces-interfaces-transports-and-routing}
 - @ref MIDI_Pipes-Routing.ino
   (Setting up automatic routes for MIDI messages between multiple MIDI interfaces, using @ref MIDI_Pipe%s)
 - @ref MIDI_Pipes-Filter.ino
   (More complicated MIDI routing rules that also filter and modify MIDI messages that travel through a MIDI_Pipe)
 - @ref Serial-Interface.ino
   (Sending and receiving MIDI messages over a serial port (e.g. for 5-pin DIN MIDI))
 - @ref BLEMIDI-Adapter.ino
   (Sending and receiving MIDI messages over Bluetooth Low Energy (LE), and relaying them over Serial)
 - @ref USBMIDI-Adapter.ino
   (Sending and receiving MIDI messages over USB, and relaying them over Serial)
 - @ref USBHostMIDI_Interface-Passthrough.ino
   (Sending and receiving MIDI messages to and from USB MIDI devices connected to a Teensy's USB Host port, and relaying them over the USB Device port)
 - @ref AppleMIDI.ino
   (Sending and receiving MIDI messages over the network (WiFi/Ethernet) using RTP-MIDI)
 - @ref Debug-MIDI-Interface.ino
   (Printing and reading MIDI messages to/from the Serial monitor for debugging purposes)
 - @ref MIDI-Monitor.ino
   (Turns the Arduino into a MIDI monitor by printing all messages it receives to the Serial monitor, and sending messages typed into the Serial monitor over MIDI)
 - @ref MIDI-Monitor-OLED.ino
   (Prints all messages that are sent and received to a small OLED display)
 - @ref Dual-MIDI-Interface.ino
   (Using multiple MIDI interfaces at the same time)

### Sending and receiving MIDI messages {#examples-midi-interfaces-sending-and-receiving-midi-messages}
 - @ref MIDI-Input.ino
   (Handling incoming MIDI messages using the @ref MIDI_Callbacks class)
 - @ref MIDI-Input-Fine-Grained.ino
   (Handling incoming MIDI messages using the @ref FineGrainedMIDI_Callbacks class)
 - @ref MIDI-Input-Fine-Grained-All-Callbacks.ino
   (Same as the previous, handles all possible message types)
 - @ref MIDI-Output.ino
   (Sends MIDI Note On/Off message every second)
 - @ref Send-MIDI-Notes.ino
   (Sends MIDI Note On/Off messages when a push button is pressed/released)
 - @ref Send-All-MIDI-Messages.ino
   (Demonstrates how to send all possible MIDI message types)
 - @ref SysEx-Send-Receive.ino
   (Sends and receives MIDI System Exclusive messages)

## MIDI output elements {#examples-midi-output-elements}
### Potentiometers & Faders {#examples-midi-output-elements-potentiometers-faders}
 - @ref Control-Change-Potentiometer.ino
   (A single analog input from a potentiometer that sends MIDI Control Change messages)
 - @ref Multiple-Control-Change-Potentiometers.ino
   (Multiple analog inputs)
 - @ref CCPotentiometer-Map.ino
   (Applying a mapping function to the analog input to account for nonlinearities or calibrate the minimum and maximum values)
 - @ref Pitch-Bend-Potentiometer.ino
   (A single analog input from a potentiometer that sends MIDI Pitch Bend messages (higher resolution than Control Change))

### Buttons & Switches {#examples-midi-output-elements-buttons-switches}
 - @ref NoteButton.ino
   (A single digital input from a momentary push button that sends MIDI Note On/Off messages)
 - @ref CCButton.ino
   (A single digital input from a momentary push button that sends MIDI Control Change messages)
 - @ref NoteButtonMatrix.ino
   (A scanning keyboard matrix for a grid of momentary push buttons that send MIDI Note On/Off messages)
 - @ref CCIncrementDecrementButtons.ino
   (Two momentary push buttons that increment/decrement a value by sending relative MIDI Control Change messages)

### Rotary Encoders {#examples-midi-output-elements-rotary-encoders}
 - @ref RotaryEncoder.ino
   (A quadrature encoder that sends position changes using relative MIDI Control Change messages)
 - @ref AbsoluteRotaryEncoder.ino
   (A quadrature encoder that sends its absolute position using MIDI Control Change messages)
 - @ref MCP23017-RotaryEncoder-No-Interrupts.ino
   (Multiple quadrature encoders connected through a MCP23017 port expander that send position changes using relative MIDI Control Change messages)
 - @ref MCP23017-RotaryEncoder-Interrupts.ino
   (Same as previous, but interrupt-driven (Teensy only))

### Program Changers {#examples-midi-output-elements-program-changers}
 - @ref PCButton.ino
   (A single digital input from a momentary push button that sends a MIDI Program Change message when pressed)
 - @ref Program-Changer.ino
   (Multiple momentary push buttons (or any other selector) that sends MIDI Program Change messages)
 - @ref ManyAddressesPCButton.ino
   (Multiple buttons that send MIDI Program Change messages, and the programs they send are arranged in multiple banks, allowing you to select many programs using a limited number of buttons)

### Other {#examples-midi-output-elements-other}
 - @ref MIDI-Controller-Finished-Example.ino
   (An example MIDI Controller that combines switches, potentiometers, rotary encoders and banks)

## MIDI input elements {#examples-midi-input-elements}
### LEDs {#examples-midi-input-elements-leds}
 - @ref 1.Note-LED.ino
   (An LED that turns on when a MIDI Note On message for a specific note is received, and off when a Note Off message is received)
 - @ref 2.Note-Range-LEDs.ino
   (Similar to the previous, but for multiple LEDs and a range of consecutive notes)
 - @ref 3.NoteLEDBar.ino
   (An LED bar graph that displays the value of received MIDI Control Change messages)
 - @ref 4.VULEDs.ino
   (An LED bar graph that displays a level meter based on Mackie Control Universal MIDI messages sent by a DAW)
 - @ref 5.Note-LED-PWM.ino
   (An LED that changes brightness depending on the MIDI velocity value received)
 - @ref 6.MAX7219-NoteLED.ino
   (Similar to @ref 1.Note-LED.ino, but using a MAX7219 8×8 LED driver)
 - @ref 9.Note-FastLED.ino
   (Similar to @ref 1.Note-LED.ino, but for addressable LEDs using the FastLED library)
 - @ref 10.Note-FastLED-ColorMapper.ino
   (Similar to previous, but customizing the colors and intensity based on the note number and the velocity)

### Other {#examples-midi-input-elements-other}
 - @ref Pitch-Bend-Value.ino
   (Keeps track of the value of incoming MIDI Pitch Bend messages)

## Banks & Selectors {#examples-banks-selectors}
 - @ref Bank.ino
   (Using banks to control the volume of 8 tracks using only 2 physical potentiometers)
 - @ref Transposer.ino
   (Using a transposer to change the pitch of some NoteButton%s using two push buttons)
 - @ref Custom-Selector-Callback.ino
   (Changing the color of an RGB LED depending on the currently selected bank)
 - @ref Bankable-Smart-Control-Change-Potentiometer.ino
   (Using potentiometers in banks can be tricky because they might send messages while cycling between banks, this issue is addressed by the @ref Bankable::CCSmartPotentiometer)

## Hardware and Extended Input/Output {#examples-hardware-and-extended-input-output}
 - @ref 1.FilteredAnalog.ino
   (Filtering an analog input using a digital low-pass filter and hysteresis)
 - @ref 1.FilteredAnalog-Advanced.ino
   (Similar to previous, with more advanced features and options)
 - @ref 2.Button.ino
   (Debounce a switch or push button)
 - @ref 3.MultiPurposeButton.ino
   (Detect long presses and double presses of a push button)
 - @ref 1.AnalogReadSerial.ino
   (Printing the values of all channels of a multiplexer to the serial monitor)
 - @ref 2.DigitalReadSerial.ino
   (Similar to previous, but for digital inputs)
 - @ref 3.FilteredAnalogReadSerial.ino
   (Similar to @ref 1.AnalogReadSerial.ino, but filters the analog inputs)
 - @ref 1.SPI-Blink.ino
   (Blink an LED that's driven by a shift register connected using SPI)
 - @ref 2.BitBang-Blink.ino
   (Similar to previous, but using ordinary GPIO pins instead of SPI)
 - @ref 2.RGB-LED-Chaser.ino
   (%Display some fancy patterns using a handful of RGB LEDs driven by shift registers)
 - @ref 1.MAX7219-Blink.ino
   (Blink an LED that's driven by a MAX7219 8×8 LED driver)
 - @ref MCP23017.ino
   (Read input from a push button and turn on/off an LED using a MCP23017 I²C port expander)
 - @ref Toggle-LEDs.ino
   (Read digital inputs from 8 push buttons connected through a multiplexer, and use them to toggle 8 LEDs on/off, using a shift register to drive them)
 - @ref MCP23017-Encoders-No-Interrupts.ino
   (Read multiple quadrature encoders using a MCP23017 port expander)
 - @ref MCP23017-Encoders-Interrupts.ino
   (Same as previous, but interrupt-driven (Teensy only))
 - @ref Pin-Change-Interrupt-Encoders.ino
   (Read multiple quadrature encoders using pin change interrupts instead of polling (AVR only))
 - @ref Timer-Interrupt-Encoders.ino
   (Use a timer interrupt to poll multiple quadrature encoders at a consistent rate (AVR only))

## Displays {#examples-displays}
 - @ref MCU-OLED-SSD1306.ino
   (Demonstrates the use of DisplayElement%s to display information from the DAW on a small OLED display)
 - @ref MCU-OLED-SSD1306-x2.ino
   (Similar to previous, with two OLED displays instead of one)

## Board-specific examples {#examples-board-specific-examples}
### Teensy {#examples-board-specific-examples-teensy}
 - @ref 1.Volume-Controlled-USB-DAC.ino
   (USB audio interface with a potentiometer to control the volume)
 - @ref 2.VU-Meter-OLED-USB-DAC.ino
   (USB audio interface with a VU meter displayed on a small OLED display)

### ESP32 {#examples-board-specific-examples-esp32}
 - @ref VU-Meter-Bridge.ino
   (Mackie Control Universal control surface with an OLED display for real-time level meters and a MIDI over BLE interface)

## Extending the library {#examples-extending-the-library}
 - @ref Custom-USB-MIDI-Backend.ino
   (Adding support for alternative MIDI over USB libraries (host and device))
 - @ref Custom-MIDI-Sender.ino
   (Customize the type of messages sent by the provided MIDI output elements)
 - @ref Custom-MIDI-Output-Element.ino
   (Adding your own MIDI output elements, recreating and then customizing the @ref NoteButton class)
 - @ref Custom-MIDI-Output-Element-Bankable.ino
   (Same as previous, but for the @ref Bankable::NoteButton class)
 - @ref Custom-MIDI-Input-Element.ino
   (Adding your own customizable MIDI input elements, recreating the @ref NoteLED class)
 - @ref Custom-Note-LED-Input-Element-Callback.ino
   (Similar to previous, but with PWM output)
 - @ref Custom-Note-LED-Input-Element-Callback-FastLED.ino
   (Similar to previous, but with support for FastLED addressable LED strips)

## Other examples {#examples-other-examples}
 - @ref Debug.ino
   (How use the debug macros for printing different kinds of debugging information)
 - @ref BlinkWithoutDelay-Timer.ino
   (Non-blocking delay using the Timer class)
 - @ref Mackie-Control-Universal-Reverse-Engineering.ino
   (Translates incoming MIDI messages to the corresponding Mackie Control Universal names and prints them to the Serial monitor)
 - @ref MIDI-Input-Callback.ino
   (Uses the MIDI input callbacks of the Control_Surface_ class to intercept MIDI input)
 - @ref Keyboard-Matrix-BCD.ino
 - @ref Bank-Button-Matrix.ino
 - @ref Encoder-Selector-Bank.ino
 - @ref Note-ManyAddresses-Transposer.ino
 - @ref One-Pot-Both-PB-and-CC.ino
 - @ref MIDI_controller-97.ino
 - @ref Transpose-Octave-NC-Button.ino
 - @ref Blink-Frequency-Buttons.ino
 - @ref Quaternion.ino
 - @ref ArraySort.ino
 - @ref Complex.ino
 - @ref CountPressedButtons.ino
 - @ref Numeric.ino
 - @ref NoteChordButton.ino
 - @ref Enable-disable.ino
 - @ref FilteredAnalog.ino
 - @ref VU-Bridge-Dual-Display.ino
 - @ref VU-Bridge.ino

