/**
 * @example   Ex.1.Shift-Register-LED-Effects.ino
 *
 * @brief   This is an example of the ShiftRegisterOut class of the Control
 *          Surface library. It creates light effects using three 8-bit shift
 *          registers and some RGB LEDs.
 *
 * Connect three daisy-chained shift registers to pins 10 (ST_CP), 11 (DS) and 
 * 13 (SH_CP).  
 * Connect 8 RGB LEDs (+ current limiting resistors) to the outputs of the shift
 * registers.
 * 
 * ```
 * 13  >───────────┬──────────────────────┬──────────────────────┐
 *         ┎━━━━━━━┷━━━━━━━┓      ┎━━━━━━━┷━━━━━━━┓      ┎━━━━━━━┷━━━━━━━┓
 *         ┃     SH_CP     ┃      ┃     SH_CP     ┃      ┃     SH_CP     ┃
 * 11  >───┨ DS        Q7S ┠──────┨ DS        Q7S ┠──────┨ DS        Q7S ┃
 *         ┃     ST_CP     ┃      ┃     ST_CP     ┃      ┃     ST_CP     ┃
 *         ┗━━━━━━━┯━━━━━━━┛      ┗━━━━━━━┯━━━━━━━┛      ┗━━━━━━━┯━━━━━━━┛
 * 10  >───────────┴──────────────────────┴──────────────────────┘
 * ```
 * 
 * The order of the colors doesn't matter. You can change them in software using
 * the `ShiftRegisterOutBase::redBit`, `ShiftRegisterOutBase::greenBit` and 
 * `ShiftRegisterOutBase::blueBit` constants.
 * 
 * If you wired the LEDs as RGB (red first, then green and then blue), the red 
 * bit is 0, the green bit is 1 and the blue bit is 2.  
 * If you wired the LEDs as BGR (blue first, then green and then red), the red
 * bit is 2, the green bit is 1 and the blue bit is 0.  
 * Other combinations are possible as well.
 * 
 * This example will turn on all red LEDs one by one, then turn them off one by
 * one, next, it will turn on and off all green LEDs in the same manner, and
 * finally the same for the blue LEDs. This is repeated indefinitely.
 * 
 * Written by Pieter P, 13-07-2018  
 * https://github.com/tttapa/Control-Surface
 */

/**
 * @example   SPIShiftRegisterBlink.ino
 *
 * @brief   This example demonstrates the use of shift registers as if they were
 *          just normal IO pins.
 * 
 * Connections:  
 * 
 *   - SS:   74HC595 ST_CP
 *   - MOSI: 74HC595 DS
 *   - CLK:  74HC595 SH_CP
 * 
 * Connect an LEDs (and series resistor) between the first output of the
 * shift register and ground. 
 * 
 * Remember to connect the enable pins the shift register to ground in order to 
 * enable it.  
 *  
 * This sketch will just blink the LED once a second.
 * 
 * Written by PieterP, 2018-09-01
 * https://github.com/tttapa/Control-Surface
 */

/**
 * @example   PushButtonsToggleLEDsShiftRegMultiplex.ino
 *
 * @brief   This example demonstrates the use of push buttons and LEDs and how 
 *          to use shift registers and analog multiplexers to save pins.  
 * 
 * Connections:  
 * 
 *   - 10: 74HC595 ST_CP
 *   - 12: 74HC595 DS
 *   - 13: 74HC595 SH_CP
 *   - 18: 74HC4067 A (COM OUT/IN)
 *   - 19: 74HC4067 S0
 *   - 20: 74HC4067 S1
 *   - 21: 74HC4067 S2
 *   - 22: 74HC4067 S3
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
 * Pressing the first button will turn on the first LED. Pressing it again will
 * turn it off again.
 * Pressing the second button will turn on the second LED. Pressing it again will
 * turn it off again, and so on.
 * 
 * Written by PieterP, 2018-08-28
 * https://github.com/tttapa/Control-Surface
 */

/**
 * @example   ShiftRegisterBlink.ino
 *
 * @brief   This example demonstrates the use of shift registers as if they were
 *          just normal IO pins.
 * 
 * Connections:  
 * 
 *   - 10: 74HC595 ST_CP
 *   - 11: 74HC595 DS
 *   - 13: 74HC595 SH_CP
 * 
 * Connect an LEDs (and series resistor) between the first output of the
 * shift register and ground. 
 * 
 * Remember to connect the enable pins the shift register to ground in order to 
 * enable it.  
 *  
 * This sketch will just blink the LED once a second.
 * 
 * Written by PieterP, 2018-08-29
 * https://github.com/tttapa/Control-Surface
 */

/**
 * @example   Getting-Started.ino
 *
 * This is the example used in the 'Getting Started' guide.  
 * Read the guide: https://github.com/tttapa/Control-Surface/blob/revision-4/doc/Getting-Started.md
 * 
 * Written by PieterP, 29-08-2018  
 * https://github.com/tttapa/Control-Surface
 */

/**
 * @example   PushButtonsLEDsShiftRegMultiplex.ino
 *
 * @brief   This example demonstrates the use of push buttons and LEDs and how 
 *          to use shift registers and analog multiplexers to save pins.  
 * 
 * Connections:  
 * 
 *   - 10: 74HC595 ST_CP
 *   - 12: 74HC595 DS
 *   - 13: 74HC595 SH_CP
 *   - 18: 74HC4051 A (COM OUT/IN)
 *   - 19: 74HC4051 S0
 *   - 20: 74HC4051 S1
 *   - 21: 74HC4051 S2
 * 
 * Connect eight momentary push buttons between the input pins of the 
 * multiplexer and ground.  
 * The internal pull-up resistor for the buttons will be enabled automatically.
 * 
 * Connect eight LEDs (and series resistors) between the eight outputs of the
 * shift register and ground. 
 * 
 * Remember to connect the enable pins of both the multiplexer and the shift
 * register to ground in order to enable them.
 *  
 * Pressing the first button will mute the first track, and will turn on the 
 * first LED.  
 * Pressing the second button will mute the second track, and will turn on the 
 * second LED, and so on.
 * 
 * Map the Arduino as a Mackie Control Universal (MCU) or equivalent in your
 * audio software.
 * 
 * Written by PieterP, 2018-08-28
 * https://github.com/tttapa/Control-Surface
 */

/**
 * @example   PushButtonsLEDs.ino
 *
 * @brief   This example demonstrates the use of push buttons and LEDs.
 * 
 * Connections:
 *  2:  momentary push button (to ground)
 *  3:  momentary push button (to ground)
 *  12: LED (+ series resistor, to ground)
 *  13: LED (+ series resistor, to ground)
 * 
 * The internal pull-up resistors for the buttons will be enabled automatically.
 *  
 * Pressing the first button will mute the first track, and will turn on the 
 * first LED.  
 * Pressing the second button will solo the first track, and will turn on the 
 * second LED.
 * 
 * Map the Arduino as a Mackie Control Universal (MCU) or equivalent in your
 * audio software.
 * 
 * Written by PieterP, 2018-08-28
 * https://github.com/tttapa/Control-Surface
 */

/**
 * @example   CCPotentiometer.ino
 *
 * @brief   This is an example of the `CCPotentiometer` class of the 
 *          Control_Surface library.
 * 
 * Connect a potentiometer to analog pin A0. This will be the MIDI channel volume of channel 1.  
 * Map it in your DAW or DJ software.
 * 
 * Written by Pieter P, 28-08-2018  
 * https://github.com/tttapa/Control-Surface
 */

/**
 * @example   OLED-Demo.ino
 *
 * @brief An extensive example demonstrating the use of DisplayElement%s
 *        to display information from the DAW on a small OLED display.
 *
 * The example displays the cursor time location, play and record status,
 * and for each of the 8 first tracks, it displays:
 * mute, solo, record ready, pan, and VU level meter with peak indicator.
 *
 * Using a BankSelector, it displays two channels at once, and you can
 * cycle through four banks to display all 8 tracks.
 *
 * There are two rotary encoders that control the pan of the selected
 * tracks, two mute, solo, record, and select buttons for the tracks,
 * and a play and record button.
 */

/**
 * @example   OLED-Demo-Audio.ino
 *
   @brief An extensive example demonstrating the use of DisplayElement%s
          to display information from the DAW on a small OLED display, and
          the use of Audio elements to play USB audio.

   The example displays the cursor time location, play and record status,
   and for each of the 8 first tracks, it displays:
   mute, solo, record ready, pan, and VU level meter with peak indicator.

   Using a BankSelector, it displays two channels at once, and you can
   cycle through four banks to display all 8 tracks.

   There are two rotary encoders that control the pan of the selected
   tracks, two mute, solo, record, and select buttons for the tracks,
   and a play and record button.

   The Teensy also acts as a USB audio interface, and it outputs the audio
   over I²S.
*/

/**
 * @example   VUMeterLEDs-USB-DAC.ino
 *
 * Connections:
 *
 * A0: wiper of a potentiometer to change the output volume
 * A1: wiper of a potentiometer to change the VU gain
 *
 * 9:  BCK (I²S)
 * 11: SCK (I²S)
 * 22: DIN (I²S)
 * 23: LRCK (I²S)
*/

/**
 * @example   VUMeterOLED-USB-DAC.ino
 *
 * Connections:
 * 
 * A0: wiper of a potentiometer to change the output volume
 * A1: wiper of a potentiometer to change the VU gain
 * 
 * 9:  BCK (I²S)
 * 11: SCK (I²S)
 * 22: DIN (I²S)
 * 23: LRCK (I²S)
 */

