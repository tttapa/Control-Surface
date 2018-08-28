/**
 * @example   Ex.1.Shift-Register-LED-Effects.ino
 *
 * This is an example of the ShiftRegisterOut class of the Control-Surface library.
 *
 * Connect three daisy-chained shift registersr to pins 11 (ST_CP), 12 (DS) and 10 (SH_CP).  
 * Connect 8 RGB LEDs (+ current limiting resistors) to the outputs of the shift registers.
 * 
 * ```
 * 10 >──────────────┬─────────────────────────────┬─────────────────────────────┐            
 *        ┎━━━━━━━━━━┷━━━━━━━━━━━┓      ┎━━━━━━━━━━┷━━━━━━━━━━━┓      ┎━━━━━━━━━━┷━━━━━━━━━━━┓
 *        ┃        SH_CP         ┃      ┃        SH_CP         ┃      ┃        SH_CP         ┃
 * 12 >───┨ Data in     Data out ┠──────┨ Data in     Data out ┠──────┨ Data in     Data out ┃
 *        ┃        ST_CP         ┃      ┃        ST_CP         ┃      ┃        ST_CP         ┃
 *        ┗━━━━━━━━━━┯━━━━━━━━━━━┛      ┗━━━━━━━━━━┯━━━━━━━━━━━┛      ┗━━━━━━━━━━┯━━━━━━━━━━━┛
 * 11 >──────────────┴─────────────────────────────┴─────────────────────────────┘            
 * ```
 * 
 * The order of the colors doesn't matter. You can change them in software using the
 * `ShiftRegisterOut::redBit`, `ShiftRegisterOut::greenBit` and `ShiftRegisterOut::blueBit`
 * constants.
 * 
 * If you wired the LEDs as RGB (red first, then green and then blue), the red bit is 0,
 * the green bit is 1 and the blue bit is 2.  
 * If you wired the LEDs as BGR (blue first, then green and then red), the red bit is 2,
 * the green bit is 1 and the blue bit is 0.  
 * Other combinations are possible as well.
 * 
 * This example will turn on all red LEDs one by one, then turn them off one by one, 
 * next, it will turn on and off all green LEDs in the same manner, and finally the
 * same for the blue LEDs. This is repeated indefinitely.
 * 
 * Written by Pieter P, 13-07-2018  
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

