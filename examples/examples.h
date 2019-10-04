/**
 * @example   "Multiple-Control-Change-Potentiometers.ino"
 * 
 * Multiple-Control-Change-Potentiometers
 * ======================================
 * 
 * This example demonstrates the use of MIDI Control Change potentiometers that
 * can be used for changing effect parameters, volumes, pan and balance
 * controls, etc. It can control almost any knob in your DAW software. 
 * This example shows how to define multiple potentiometers using an array.
 *
 * @boards  AVR, AVR USB, Teensy 3.x
 * 
 * Connections
 * -----------
 * 
 * - A0: wiper of the first potentiometer
 * - A1: wiper of the second potentiometer
 * - ...
 * - A5: wiper of the sixth potentiometer
 * 
 * Connect the left terminal of the potentiometers to ground, and the right one
 * to V<sub>CC</sub>.
 * 
 * Behavior
 * --------
 * 
 * - When you turn a potentiometer, you should receive MIDI Control Change
 *   events, with a value between 0 and 127.
 * - The analog inputs are filtered, so there shouldn't be any noise on the 
 *   position. If there is, check your wiring, and make sure that the resistance
 *   of the potentiometers isn't too high (10 kΩ is ideal).
 * 
 * Mapping
 * -------
 * 
 * Select the Arduino as a custom MIDI controller in your DAW, and use the 
 * MIDI learn option to assign the potentiometers to a function.
 * 
 * Written by PieterP, 2019-08-13  
 * https://github.com/tttapa/Control-Surface
 */

/**
 * @example   "Pitch-Bend-Potentiometer.ino"
 * 
 * Pitch-Bend-Potentiometer
 * ========================
 * 
 * This example demonstrates the use of MIDI Pitch Bend potentiometers that
 * can be used for changing effect parameters, volumes, pan and balance
 * controls, etc. It can control almost any knob in your DAW software.
 *
 * @boards  AVR, AVR USB, Teensy 3.x, ESP32
 * 
 * Connections
 * -----------
 * 
 * - A0: wiper of a potentiometer
 * 
 * Connect the left terminal of the potentiometer to ground, and the right one
 * to V<sub>CC</sub>.
 * 
 * Behavior
 * --------
 * 
 * - When you turn the potentiometer, you should receive MIDI Pitch Bend
 *   events on channel 1, with a value between -8192 and +8191.
 * - The analog input is filtered, so there shouldn't be any noise on the 
 *   position. If there is, check your wiring, and make sure that the resistance
 *   of the potentiometer isn't too high (10 kΩ is ideal).
 * 
 * Mapping
 * -------
 * 
 * Select the Arduino as a custom MIDI controller in your DAW, and use the 
 * MIDI learn option to assign the potentiometer to a function.
 * 
 * Written by PieterP, 2019-08-23  
 * https://github.com/tttapa/Control-Surface
 */

/**
 * @example   "Control-Change-Potentiometer.ino"
 * 
 * Control-Change-Potentiometer
 * ============================
 * 
 * This example demonstrates the use of MIDI Control Change potentiometers that
 * can be used for changing effect parameters, volumes, pan and balance
 * controls, etc. It can control almost any knob in your DAW software.
 *
 * @boards  AVR, AVR USB, Teensy 3.x, ESP32
 * 
 * Connections
 * -----------
 * 
 * - A0: wiper of a potentiometer
 * 
 * Connect the left terminal of the potentiometer to ground, and the right one
 * to V<sub>CC</sub>.
 * 
 * Behavior
 * --------
 * 
 * - When you turn the potentiometer, you should receive MIDI Control Change
 *   events, with a value between 0 and 127.
 * - The analog input is filtered, so there shouldn't be any noise on the 
 *   position. If there is, check your wiring, and make sure that the resistance
 *   of the potentiometer isn't too high (10 kΩ is ideal).
 * 
 * Mapping
 * -------
 * 
 * Select the Arduino as a custom MIDI controller in your DAW, and use the 
 * MIDI learn option to assign the potentiometer to a function.
 * 
 * Written by PieterP, 2019-08-13  
 * https://github.com/tttapa/Control-Surface
 */

/**
 * @example   "NoteButton.ino"
 * 
 * NoteButton
 * ==========
 * 
 * This is an example that demonstrates the use of MIDI Note Buttons that can be
 * used for triggering samples, mute/solo/rec buttons, play/pause/stop buttons, 
 * etc. It can control almost any push button control in your DAW software.
 *
 * @boards  AVR, AVR USB, Teensy 3.x, ESP32
 * 
 * Connections
 * -----------
 * 
 * - 5: momentary push button (to ground)
 * 
 * The internal pull-up resistor for the button will be enabled automatically.
 * 
 * Behavior
 * --------
 * 
 * - When the button on pin 5 is pressed, a MIDI Note On message is sent for
 *   note C4.
 * - When the button on pin 5 is released, a MIDI Note Off message is sent for 
 *   note C4.
 * 
 * Mapping
 * -------
 * 
 * Select the Arduino as a custom MIDI controller in your DAW, and use the 
 * MIDI learn option to assign the button to a function.
 * 
 * Written by PieterP, 2019-08-07  
 * https://github.com/tttapa/Control-Surface
 */

/**
 * @example   "CCButton.ino"
 * 
 * CCButton
 * ========
 * 
 * This is an example that demonstrates the use of MIDI Control Change Buttons.
 *
 * @boards  AVR, AVR USB, Teensy 3.x, ESP32
 * 
 * Connections
 * -----------
 * 
 * - 5: momentary push button (to ground)
 * 
 * The internal pull-up resistor for the button will be enabled automatically.
 * 
 * Behavior
 * --------
 * 
 * - When the button on pin 5 is pressed, a MIDI Control Change message with a 
 *   value of 0x7F (127) is sent for General Purpose Controller #1.
 * - When the button on pin 5 is released, a MIDI Control Change message with a 
 *   value of 0x00 (0) is sent for General Purpose Controller #1
 * 
 * Mapping
 * -------
 * 
 * Select the Arduino as a custom MIDI controller in your DAW, and use the 
 * MIDI learn option to assign the button to a function.
 * 
 * Written by PieterP, 2019-09-29  
 * https://github.com/tttapa/Control-Surface
 */

/**
 * @example   "CCIncrementDecrementButtons.ino"
 * 
 * CCIncrementDecrementButtons
 * ===========================
 * 
 * This is an example that demonstrates the use of Increment and 
 * Decrement buttons that can be used for scrolling through a song
 * list, tuning effects, etc.  
 * You can compare it to a rotary encoder, but with two buttons instead of an 
 * encoder.
 *
 * @boards  AVR, AVR USB, Teensy 3.x, ESP32
 *
 * Connections
 * -----------
 * 
 * - 5: momentary push button (to ground)
 * - 6: momentary push button (to ground)
 * 
 * The internal pull-up resistors for the buttons will be enabled automatically.
 * 
 * Behavior
 * --------
 * 
 * - If the button on pin 5 is pressed, a MIDI CC increment message is sent.
 * - If the button on pin 6 is pressed, a MIDI CC decrement message is sent.
 * - The controller number `MCU::V_POT_1` will be used.
 * - If either of the buttons is pressed down for a longer time, it will keep on
 *   sending increment/decrement messages quickly until you release it (like a 
 *   key on a computer keyboard).
 *   This is to make it easier to scroll a large amount, for example.
 *   The timing can be changed by tweaking @ref LONG_PRESS_DELAY and 
 *   @ref LONG_PRESS_REPEAT_DELAY in Settings/Settings.hpp.
 * - If both buttons are pressed simultaneously, a MIDI Note On event is sent on
 *   note `MCU::V_POT_SELECT_1`.  
 *   (This is optional, you can just leave out the final argument to the 
 *   constructor, and it won't send anything when you press both buttons at 
 *   once).
 * - The multiplier can be used to increment multiple ticks per each time you 
 *   press a button. This is useful if you have to press the button for a very
 *   long time to get a substantial change.
 * 
 * Map the Arduino as a Mackie Control Universal (MCU) or equivalent in your
 * audio software.
 * 
 * Written by PieterP, 2019-01-26  
 * https://github.com/tttapa/Control-Surface
 */

/**
 * @example   "MIDI-Controller-Finished-Example.ino"
 * 
 * MIDI-Controller-Finished-Example
 * ================================
 * 
 * This is the code I used for my specific MIDI controller.  
 * 
 * @boards  Leonardo
 * 
 * Connections
 * -----------
 * 
 * I used an Arduino Leonardo with:
 * - 4 faders on A0-A3
 * - 8 potentiometers on A4-A11
 * - 1 rotary encoder on pins 0-1
 * - 4 toggle switches on pins 2, 3, 5 and 7
 * - 1 toggle switch (for bank select) on pin 11
 * - 1 LED on pin 13
 *
 * Behavior
 * --------
 * 
 * - When bank select is 0, faders 1-4 are channel volumes 1-4, potSide 1-4 are 
 *   pan/balance of channel 1-4, switches 1-4 are mute channel 5-8.
 * - When bank select is 1, faders 1-4 are channel volumes 5-8, potSide 1-4 are 
 *   pan/balance of channel 5-8, switches 1-4 are mute channel 1-4.
 * - PotTop 1-4 are not in the bank, because I use them as effect or EQ
 *   parameters, and they are channel independent.
 * - Enc1 is used for scrolling.
 *
 * Mapping
 * -------
 * 
 * Create a custom mapping using the MIDI learn option in your DAW.
 *
 * Written by PieterP, 07-09-2017
 * https://github.com/tttapa/Control-Surface
 */

/**
 * @example   "1.First-Output.ino"
 * 
 * 1.First-Output
 * ==============
 *
 * This is the first example used in the 'Getting Started' guide:
 * https://tttapa.github.io/Control-Surface/Doc/Doxygen/dd/dcc/md_Getting-Started.html
 * 
 * @boards  AVR, AVR USB, Teensy 3.x, ESP32
 *
 *  Written by PieterP, 2018-08-29  
 *  https://github.com/tttapa/Control-Surface
 */

/**
 * @example   "2.First-Input.ino"
 * 
 * 2.First-Input
 * =============
 *
 * This is the second example used in the 'Getting Started' guide:
 * https://tttapa.github.io/Control-Surface/Doc/Doxygen/dd/dcc/md_Getting-Started.html
 * 
 * @boards  AVR, AVR USB, Teensy 3.x, ESP32
 * 
 * @htmlonly
 * <iframe width="560" height="315"
 * src="https://www.youtube.com/embed/u1IbYXPT6mM?loop=1" frameborder="0"
 * allow="accelerometer; autoplay; encrypted-media; gyroscope;
 * picture-in-picture" allowfullscreen></iframe>
 * @endhtmlonly
 *
 * Written by PieterP, 2018-08-29  
 * https://github.com/tttapa/Control-Surface
 */

/**
 * @example   "2.RGB-LED-Chaser.ino"
 * 
 * 2.RGB-LED-Chaser
 * ================
 *
 * This is an example of the ShiftRegisterOut class of the Control Surface 
 * library. It creates light effects using three 8-bit shift registers and some
 * RGB LEDs.
 *
 * @boards  AVR, AVR USB, Teensy 3.x, ESP32
 *
 * Connections
 * -----------
 *
 * Connect three daisy-chained shift registers to the SPI pins.  
 * Connect 8 RGB LEDs (+ current limiting resistors) to the outputs of the shift
 * registers.
 * 
 * ```
 * SCK   >───────────┬──────────────────────┬──────────────────────┐
 *           ┏━━━━━━━┷━━━━━━━┓      ┏━━━━━━━┷━━━━━━━┓      ┏━━━━━━━┷━━━━━━━┓
 *           ┃     SH_CP     ┃      ┃     SH_CP     ┃      ┃     SH_CP     ┃
 * MOSI  >───┨ DS        Q7S ┠──────┨ DS        Q7S ┠──────┨ DS        Q7S ┃
 *           ┃     ST_CP     ┃      ┃     ST_CP     ┃      ┃     ST_CP     ┃
 *           ┗━━━━━━━┯━━━━━━━┛      ┗━━━━━━━┯━━━━━━━┛      ┗━━━━━━━┯━━━━━━━┛
 * SS    >───────────┴──────────────────────┴──────────────────────┘
 * ```
 * 
 * The order of the colors doesn't matter. You can change them in software using
 * the `ShiftRegisterOutRGB::redBit`, `ShiftRegisterOutRGB::greenBit` and 
 * `ShiftRegisterOutRGB::blueBit` constants.
 * 
 * If you wired the LEDs as RGB (red first, then green and then blue), the red 
 * bit is 0, the green bit is 1 and the blue bit is 2.  
 * If you wired the LEDs as BGR (blue first, then green and then red), the red
 * bit is 2, the green bit is 1 and the blue bit is 0.  
 * Other combinations are possible as well.
 * 
 * Behavior
 * --------
 * 
 * This example will turn on all red LEDs one by one, then turn them off one by
 * one, next, it will turn on and off all green LEDs in the same manner, and
 * finally the same for the blue LEDs. This is repeated indefinitely.
 * 
 * Mapping
 * -------
 * 
 * None.
 * 
 * Demo
 * ----
 * 
 * @htmlonly
 * <iframe width="560" height="315"
 * src="https://www.youtube.com/embed/7ywcTTdnseU?loop=1" frameborder="0"
 * allow="accelerometer; autoplay; encrypted-media; gyroscope;
 * picture-in-picture" allowfullscreen></iframe>
 * @endhtmlonly
 * 
 * Written by Pieter P, 2018-07-13  
 * https://github.com/tttapa/Control-Surface
 */

/**
 * @example   "2.BitBang-Blink.ino"
 * 
 * 2.BitBang-Blink
 * ===============
 *
 * This example demonstrates the use of shift registers as if they were just
 * normal IO pins. This version uses bit-banging to drive the  shift register. 
 * You should probably be using the SPI version instead.
 * 
 * @boards  AVR, AVR USB, Teensy 3.x, ESP32
 * 
 * Connections
 * -----------
 * 
 *   - 10: 74HC595 ST_CP
 *   - 11: 74HC595 DS
 *   - 13: 74HC595 SH_CP
 * 
 * Connect an LED (and series resistor) between the first output of the
 * shift register and ground.
 * 
 * Remember to connect the enable pin the shift register to ground in order to 
 * enable it.  
 * 
 * Behavior
 * --------
 * 
 * This sketch will just blink the LED once a second.
 * 
 * Mapping
 * -------
 * 
 * None.
 * 
 * Written by PieterP, 2018-09-01
 * https://github.com/tttapa/Control-Surface
 */

/**
 * @example   "1.SPI-Blink.ino"
 * 
 * 1.SPI-Blink
 * ===========
 *
 * This example demonstrates the use of shift registers as if they were just
 * normal IO pins. The SPI interface is used because it's easy and fast.
 * 
 * @boards  AVR, AVR USB, Teensy 3.x, ESP32
 * 
 * Connections
 * -----------
 * 
 *   - SS:   74HC595 ST_CP
 *   - MOSI: 74HC595 DS
 *   - SCK:  74HC595 SH_CP
 * 
 * Connect an LED (and series resistor) between the first output of the
 * shift register and ground.
 * 
 * Remember to connect the enable pin the shift register to ground in order to 
 * enable it.  
 * 
 * Behavior
 * --------
 * 
 * This sketch will just blink the LED once a second.
 * 
 * Mapping
 * -------
 * 
 * None.
 * 
 * Written by PieterP, 2018-09-01
 * https://github.com/tttapa/Control-Surface
 */

/**
 * @example   "Toggle-LEDs.ino"
 * 
 * Toggle-LEDs
 * ===========
 *
 * This example demonstrates the use of push buttons and LEDs and how to use
 * shift registers and analog multiplexers to save pins.
 * 
 * @boards  AVR, AVR USB, Teensy 3.x, ESP32
 * 
 * Connections
 * -----------
 * 
 *   - SS:   74HC595 ST_CP
 *   - MOSI: 74HC595 DS
 *   - SCK:  74HC595 SH_CP
 *   - 2: 74HC4067 A (COM OUT/IN)
 *   - 3: 74HC4067 S0
 *   - 4: 74HC4067 S1
 *   - 5: 74HC4067 S2
 *   - 6: 74HC4067 S3
 * 
 * Connect 16 momentary push buttons between the input pins of the 
 * multiplexer and ground.  
 * The internal pull-up resistor for the buttons will be enabled automatically,
 * so no external resistors are necessary.
 * 
 * Connect 16 LEDs (and series resistors) between the eight outputs of the
 * two shift registers and ground. 
 * 
 * Remember to connect the enable pins of both the multiplexer and the shift
 * register to ground in order to enable them.  
 * Connect the serial data output of the first shift register (QH' or Q7S) to
 * the serial input of the second shift register.
 * 
 * Behavior
 * --------
 *  
 * Pressing the first button will turn on the first LED. Pressing it again will
 * turn it off again.
 * Pressing the second button will turn on the second LED. Pressing it again 
 * will turn it off again, and so on.
 * 
 * Mapping
 * -------
 * 
 * None.
 * 
 * Written by PieterP, 2018-08-28  
 * https://github.com/tttapa/Control-Surface
 */

/**
 * @example   "1.AnalogReadSerial.ino"
 * 
 * 1.AnalogReadSerial
 * ==================
 *
 * This is an example of the AnalogMultiplex class. It prints the values of all
 * 16 inputs of a multiplexers to the serial monitor.
 * 
 * @boards  AVR, AVR USB, Teensy 3.x, ESP32
 * 
 * Connections
 * -----------
 * 
 *  - A0: CD74HC4067 signal pin ("common input/output")
 *  - 3:  CD74HC4067 address pin S0
 *  - 4:  CD74HC4067 address pin S1
 *  - 5:  CD74HC4067 address pin S2
 *  - 6:  CD74HC4067 address pin S3
 *  
 * Optionally you can connect the enable pin as well, this is useful
 * if you want to use multiple multiplexers with the same address 
 * lines and the same analog input. Otherwise, just connect the enable
 * pin to ground.
 * 
 * If you are using a 3-bit multiplexer, like the CD74HC4051, you can
 * uncomment the code specific to this multiplexer, and use only three
 * address pins.
 *  
 * Behavior
 * --------
 * 
 * Connect some potentiometers or other analog sensors to the 16 inputs
 * of the multiplexer, and open the serial monitor (CTRL+SHIFT+M) or the
 * serial plotter (CTRL+SHIFT+L). You should see all 16 signals printed
 * or plotted.
 * 
 * Mapping
 * -------
 * 
 * None.
 * 
 * Written by Pieter P, 31-01-2019  
 * https://github.com/tttapa/Control-Surface
 */

/**
 * @example   "2.DigitalReadSerial.ino"
 * 
 * 2.DigitalReadSerial
 * ===================
 *
 * This is an example of the AnalogMultiplex class. It prints the states of all
 * 16 switches connected to a multiplexers to the serial monitor.
 * 
 * @boards  AVR, AVR USB, Teensy 3.x, ESP32
 * 
 * Connections
 * -----------
 * 
 *  - 2: CD74HC4067 signal pin ("common input/output")
 *  - 3: CD74HC4067 address pin S0
 *  - 4: CD74HC4067 address pin S1
 *  - 5: CD74HC4067 address pin S2
 *  - 6: CD74HC4067 address pin S3
 *  
 * Optionally you can connect the enable pin as well, this is useful
 * if you want to use multiple multiplexers with the same address 
 * lines and the same analog input. Otherwise, just connect the enable
 * pin to ground.
 * 
 * If you are using a 3-bit multiplexer, like the CD74HC4051, you can
 * uncomment the code specific to this multiplexer, and use only three
 * address pins.
 * 
 * Connect a switch or push button between each input pin of the multiplexer and
 * ground. A pull-up resistor is not necessary, because we'll use the internal 
 * one.
 *  
 * Behavior
 * --------
 * 
 * Open the serial monitor (CTRL+SHIFT+M) or the serial plotter (CTRL+SHIFT+L),
 * and press some buttons, you should see all 16 signals printed or plotted.
 * 
 * Mapping
 * -------
 * 
 * None.
 * 
 * Written by Pieter P, 2019-08-08   
 * https://github.com/tttapa/Control-Surface
 */

/**
 * @example   "Debug-MIDI-Interface.ino"
 * 
 * Debug-MIDI-Interface
 * ====================
 * 
 * This example demonstrates how to use the MIDI interface for debugging.
 *
 * @boards  AVR, AVR USB, Teensy 3.x, ESP32
 * 
 * Connections
 * -----------
 * 
 * - 5: momentary push button (to ground)
 * 
 * The internal pull-up resistor for the button will be enabled automatically.
 * 
 * Behavior
 * --------
 * 
 * - Upload the sketch and open the Serial monitor.
 * - Set the line ending to new line or new line and carriage return.
 * - When the button on pin 5 is pressed, a MIDI Note On message is sent for
 *   note C4, and you'll see that event printed in the Serial monitor.
 * - When the button on pin 5 is released, a MIDI Note Off message is sent for 
 *   note C4, and you'll also see that in the monitor.
 * - When you type in "90 3C 7F" (without the quotes), and press enter, the 
 *   built-in LED will light up.
 * - When you type in "80 3C 7F", and press enter, the LED will turn off again.
 * 
 * Mapping
 * -------
 * 
 * None.
 * 
 * Written by PieterP, 2019-08-07  
 * https://github.com/tttapa/Control-Surface
 */

/**
 * @example   "SysEx-Send-Receive.ino"
 * 
 * SysEx-Send-Receive
 * ==================
 *
 * Example showing how to send and receive MIDI System Exclusive messages.
 * 
 * @boards  AVR, AVR USB, Teensy 3.x, ESP32
 */

/**
 * @example   "Dual-MIDI-Interface.ino"
 * 
 * Dual-MIDI-Interface
 * ===================
 * 
 * This example demonstrates how to group together two or more MIDI interfaces,
 * so you can send the MIDI output over both MIDI USB and 5-pin DIN MIDI, for 
 * example. MIDI input is received from all interfaces as well.
 *
 * @boards  AVR USB, Teensy 3.x
 * 
 * Connections
 * -----------
 * 
 * - A0: wiper of a potentiometer
 * 
 * Connect the left terminal of the potentiometer to ground, and the right one
 * to V<sub>CC</sub>.
 * 
 * Behavior
 * --------
 * 
 * - When you turn the potentiometer, you should receive MIDI Control Change
 *   events on both the Serial MIDI interface and the USB MIDI interface.
 * - When a MIDI Note event for middle C is received on either MIDI interface,
 *   the state of the built-in LED will change accordingly.
 * 
 * Mapping
 * -------
 * 
 * - None
 * 
 * Written by PieterP, 2019-08-22  
 * https://github.com/tttapa/Control-Surface
 */

/**
 * @example   "1.Note-LED.ino"
 * 
 * 1.Note-LED
 * ==========
 *
 * This example demonstrates the use of LEDs that respond to incoming  MIDI
 * note events.
 * 
 * @boards  AVR, AVR USB, Teensy 3.x, ESP32
 * 
 * Connections
 * -----------
 * 
 * None.
 * 
 * Behavior
 * --------
 * 
 * If a MIDI Note On event for note 0x3C (C4 or middle C) is sent, the built-in 
 * LED will light up, if a Note Off event for that note is sent, the LED will 
 * turn off.  
 * (A Note On event with a velocity of zero also counts as a Note Off event.)
 * 
 * Mapping
 * -------
 * 
 * Route the MIDI output of a MIDI keyboard to the Arduino's MIDI input. Then
 * play a middle C on the keyboard.
 * 
 * Written by PieterP, 2019-02-07  
 * https://github.com/tttapa/Control-Surface
 */

/**
 * @example   "Mackie-Control-Universal-Reverse-Engineering.ino"
 * 
 * Mackie-Control-Universal-Reverse-Engineering
 * ============================================
 *
 * Sketch that reads the MIDI input from the USB MIDI Interface and prints it to the
 * Serial monitor, including the Mackie Control Universal note names.
 * 
 * This is useful to discover what kinds of messages your DAW is sending.
 * 
 * @boards  AVR USB, Teensy 3.x
 */

/**
 * @example   "Bank-Button-Matrix.ino"
 * 
 * Bank-Button-Matrix
 * ==================
 *
 * <https://github.com/tttapa/Control-Surface/issues/38>
 */

/**
 * @example   "MIDI-Input-Callback.ino"
 * 
 * MIDI-Input-Callback
 * ===================
 * 
 * This is an example on how to attach your own callbacks for receiving MIDI 
 * input data.
 *
 * @boards  AVR, AVR USB, Teensy 3.x, ESP32
 * 
 * Connections
 * -----------
 * 
 * None.
 * 
 * Behavior
 * --------
 * 
 * - When MIDI messages are sent to the USB interface, they are printed to the 
 *   Serial port.
 * 
 * Mapping
 * -------
 * 
 * None.
 * 
 * Written by PieterP, 2019-08-07  
 * https://github.com/tttapa/Control-Surface
 */

/**
 * @example   "1.Volume-Controlled-USB-DAC.ino"
 * 
 * 1.Volume-Controlled-USB-DAC
 * ===========================
 *
 * This is an example of the VolumeControl class of the Control Surface library.
 * It acts as a USB Audio DAC (Digital-to-Analog Converter).
 * 
 * @boards Teensy 3.x
 * 
 * Connections
 * -----------
 * 
 * - A0: wiper of a potentiometer to change the output volume
 * - 9:  BCK (I²S)
 * - 11: SCK (I²S)
 * - 22: DIN (I²S)
 * - 23: LRCK (I²S)
 * 
 * Select a USB audio option from the Tools menu in the IDE.
 * 
 * Behavior
 * --------
 * 
 * Upload the sketch, and select the Control Surface as the audio output of your
 * computer. Connect the output of the DAC to a pair of headphones or powered
 * speakers, and play some music.  
 * You can now adjust the volume using the potentiometer.
 * 
 * Mapping
 * -------
 * 
 * None.
 * 
 * Written by PieterP, 2019-08-09  
 * https://github.com/tttapa/Control-Surface 
 */

/**
 * @example   "2.VU-Meter-OLED-USB-DAC.ino"
 * 
 * 2.VU-Meter-OLED-USB-DAC
 * =======================
 *
 * This example shows the usage of the AudioVU and AnalogVUDisplay classes of 
 * the Control Surface library.  
 * 
 * It displays two analog-style VU meters on two 128×64 OLED displays.  
 * The VU meters imitate the inertia and ballistics of real analog VU meters.
 * 
 * It acts as a USB Audio DAC (Digital-to-Analog Converter).
 * 
 * @boards  Teensy 3.x
 * 
 * Connections
 * -----------
 * 
 * - A0: wiper of a potentiometer to change the output volume
 * - A1: wiper of a potentiometer to change the VU gain
 * 
 * - 9:  BCK (I²S)
 * - 11: SCK (I²S)
 * - 22: DIN (I²S)
 * - 23: LRCK (I²S)
 * 
 * - 7:  OLED Data/D1 (SPI MOSI)
 * - 13: OLED Clock/D0 (SPI SCK)
 * - 17: OLED Data/Command
 * - 10: Left OLED Cable Select
 * - 18: Right OLED Cable Select
 * 
 * Add a capacitor between the reset pin of the display and ground, and a 
 * resistor from reset to 3.3V. The values are not critical, 0.1µF and 10kΩ 
 * work fine.  
 * You do need some way to reset the display, without it, it won't work.  
 * Alternatively, you could use an IO pin from the Teensy to reset the 
 * display, but this just "wastes" a pin.
 * 
 * Behavior
 * --------
 * 
 * Upload the sketch, and select the Control Surface as the audio output of your
 * computer. Connect the output of the DAC to a pair of headphones or powered
 * speakers, and play some music.  
 * You should see the VU meters come to life and jump around to the music.
 * 
 * You can now adjust the volume using the potentiometer on pin A0, and the 
 * gain/sensitivity of the meters using the potentiometer on pin A1.
 * 
 * Mapping
 * -------
 * 
 * None.
 * 
 * Demo
 * ----
 * 
 * @todo Add a demo video.
 * 
 * Written by PieterP, 2019-08-09  
 * https://github.com/tttapa/Control-Surface
 */

/**
 * @example   "VU-Meter-Bridge.ino"
 * 
 * VU-Meter-Bridge
 * ===============
 *
 * This is an example on how to use an OLED display to display the VU meters and
 * mute/solo states of the eight first tracks, by using the Arduino as a Mackie
 * Control Universal. This is an example modified to work with the ESP32 only, 
 * using MIDI over Bluetooth Low Energy.  
 * 
 * @boards  ESP32
 * 
 * Connections
 * -----------
 * 
 * This example drives two SSD1306 OLED displays over SPI
 *  - SCK:  SSD1306 D0
 *  - MOSI: SSD1306 D1
 *  - 15:   SSD1306 DC
 *  - 2:    SSD1306 CS
 * 
 * Add a capacitor between the reset pin of the display and ground, and a 
 * resistor from reset to 3.3V. The values are not critical, 0.1µF and 10kΩ 
 * work fine.  
 * You do need some way to reset the display, without it, it won't work.  
 * Alternatively, you could use an IO pin from the ESP32 to reset the 
 * display, but this just "wastes" a pin.
 * 
 * Behavior
 * --------
 * 
 * The first display should now display the level meters and mute/solo states
 * of the first 8 tracks.
 * 
 * Mapping
 * -------
 * 
 * Map "Control Surface" as a Mackie Control Universal unit in your DAW.
 * 
 * @note    There seem to be some differences in the way some applications 
 *          handle VU meters: some expect the hardware to decay automatically,
 *          some don't.  
 *          If you notice that the meters behave strangely, try both decay 
 *          options of the MCU::VU class, or try a different decay time.
 * 
 * Demo
 * ----
 * 
 * @htmlonly
 * <iframe width="560" height="315"
 * src="https://www.youtube.com/embed/_NJh6LyJjU8?start=25" frameborder="0"
 * allow="accelerometer; autoplay; encrypted-media; gyroscope;
 * picture-in-picture" allowfullscreen></iframe>
 * @endhtmlonly
 * 
 * Written by PieterP, 2019-08-09  
 * https://github.com/tttapa/Control-Surface
 */

