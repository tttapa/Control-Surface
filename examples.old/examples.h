/**
 * @example Ex.04.Switch.ino
 * 
 * This is an example of the DigitalLatch class of the Control_Surface library.
 * 
 * Connect a toggle switch to digital pin 2. Connect the other pin of the switch to the ground, 
 * a pull-up resistor is not necessary, because the internal one will be used.  
 * The switch will briefly play MIDI note C4 when flipped.  
 * Map it in your DAW or DJ software.
 * 
 * Written by tttapa, 08/09/2017  
 * https://github.com/tttapa/Control-Surface
*/

/**
 * @example Ex.16.ProgramSelector.ino
 * 
 * This is an example of the ProgramSelector class of the Control_Surface library.
 * 
 * Connect two push buttons between pins 11 and 12 and ground,
 * pull-up resistors are not necessary, because the internal ones will be used.
 * 
 * When the buttons are pressed, the patch number changes (see Selector for 
 * more information and the use of different selecting modes), and a 
 * MIDI Program Change event is sent.
 * 
 * Written by Pieter P, 11-04-2018   
 * https://github.com/tttapa/Control-Surface
*/

/**
 * @example Ex.01.Potentiometer.ino
 * 
 * This is an example of the AnalogCC class of the Control_Surface library.
 * 
 * Connect a potentiometer to analog pin A0. This will be the MIDI channel volume of channel 1.  
 * Map it in your DAW or DJ software.
 * 
 * Written by Pieter P, 08-09-2017  
 * https://github.com/tttapa/Control-Surface
 */

/**
 * @example Ex.05.HiRes-Potentiometer.ino 
 * 
 * This is an example of the AnalogPB class of the Control_Surface library.
 * 
 * Connect a potentiometer to analog pin A0. It will send MIDI Pitch Bend messages on channel 1.  
 * Map it in your DAW or DJ software.
 * 
 * Written by Pieter P, 08-09-2017  
 * https://github.com/tttapa/Control-Surface
 */

/**
 * @example Ex.09.Bank-Calibrate.ino
 * 
 * This is an example of the Bank class of the Control_Surface library.
 * 
 * Connect two potentiometers to analog pins A0 and A1,
 * and two pushbuttons to pins 2 and 3.
 * 
 * Connect a toggle switch to pin 11.
 * 
 * When the toggle switch is turned off:
 *   - Potentiometer A is channel volume of track 1   &emsp;   (Controller number 0x07, MIDI channel 1)
 *   - Potentiometer B is channel volume of track 2   &emsp;   (Controller number 0x07, MIDI channel 2)
 *   - Mute button A is the mute button for track 1   &emsp;   (Note number 0x10, MIDI channel 1)
 *   - Mute button B is the mute button for track 2   &emsp;   (Note number 0x11, MIDI channel 1)
 * 
 * When the toggle switch is turned on:
 *   - Potentiometer A is channel volume of track 3   &emsp;   (Controller number 0x07, MIDI channel 3)
 *   - Potentiometer B is channel volume of track 4   &emsp;   (Controller number 0x07, MIDI channel 4)
 *   - Mute button A is the mute button for track 3   &emsp;   (Note number 0x12, MIDI channel 1)
 *   - Mute button B is the mute button for track 4   &emsp;   (Note number 0x13, MIDI channel 1)
 * 
 * This allows you to control multiple tracks with only a limited amount of physical potentiometers and buttons
 * 
 * Map accordingly in your DAW or DJ software.
 * 
 * Set 'minimumValue' to the analog value read when the potentiometer is in the minimum position.  
 * Set 'maximumValue' to the analog value read when the potentiometer is in the maximum position.
 *
 * Note: It is recommended to use a BankSelector (See Ex.07.BankSelectors) instead of changing the bank setting manually.
 *
 * Written by Pieter P, 20-09-2017  
 * https://github.com/tttapa/Control-Surface
 */

/**
 * @example Ex.10.Debug-MIDI.ino
 * 
 * This is an example of the debug functionality of the Control_Surface library.
 * 
 * Connect a push buttons to digital pin 2. Connect the other pin of the button to the ground, 
 * a pull-up resistor is not necessary, because the internal one will be used.  
 * This button will play MIDI note C4 when pressed.  
 * 
 * Open the Serial Monitor in the Arduino IDE (CTRL+SHIFT+M) and press the button.  
 * You should see messages like:
 *
 * - Note On    Channel: 1  Data 1: 0x3C  Data 2: 0x7F
 * - Note Off    Channel: 1  Data 1: 0x3C  Data 2: 0x7F
 * 
 * You can also send MIDI messages in hexadecimal format:  
 * For example: `90 3C 7F` (note on, note 0x3C, velocity 0x7F) should turn on the built-in LED.  
 * `80 3C 7F` (note off, note 0x3C, velocity 0x7F) should turn off the built-in LED.
 *
 * Written by Pieter P, 20-09-2017  
 * https://github.com/tttapa/Control-Surface
 */

/**
 * @example Ex.12.AnalogMultiplex.ino
 * 
 * This is an example of the AnalogMultiplex class of the Control_Surface library.
 * 
 * Connect an analog multiplexer to analog pin A0. Connect the address lines to pins 2, 3 and 4.  
 * Connect 8 potentiometers to the inputs of the multiplexer.  
 * These will be the channel volumes
 * of MIDI channels 1 through 8.  
 * Map accordingly in your DAW or DJ software.
 *
 * Written by Pieter P, 20-09-2017  
 * https://github.com/tttapa/Control-Surface
*/

/**
 * @example Ex.08.Potentiometer-Calibrate.ino
 * 
 * This is an example of the AnalogCC and AnalogPB classes of the Control_Surface library.
 * 
 * Connect a potentiometer to analog pin A0. This will be the MIDI channel volume of channel 1.
 * 
 * Map it in your DAW or DJ software.
 * 
 * Set 'minimumValue' to the analog value read when the potentiometer is in the minimum position.  
 * Set 'maximumValue' to the analog value read when the potentiometer is in the maximum position.
 * 
 * Written by Pieter P, 20-09-2017  
 * https://github.com/tttapa/Control-Surface
 */

/**
 * @example Ex.07.BankSelectors.ino
 * 
 * This is an example of the BankSelector class of the Control_Surface library.
 *
 * Connect two potentiometers to analog pins A0 and A1,
 * and two pushbuttons to pins 2 and 3.
 *
 * Connect push buttons to pins 11 and 12, and 4 LEDs (+ current limiting resitors) to pins 4, 5, 6 and 7.
 *
 * When bank 1 is selected:
 *   - Potentiometer A is channel volume of track 1   &emsp;   (Controller number 0x07, MIDI channel 1)
 *   - Potentiometer B is channel volume of track 2   &emsp;   (Controller number 0x07, MIDI channel 2)
 *   - Mute button A is the mute button for track 1   &emsp;   (Note number 0x10, MIDI channel 1)
 *   - Mute button B is the mute button for track 2   &emsp;   (Note number 0x11, MIDI channel 1)
 *   - The LED on pin 4 lights up.
 *
 * When bank 2 is selected:
 *   - Potentiometer A is channel volume of track 3   &emsp;   (Controller number 0x07, MIDI channel 3)
 *   - Potentiometer B is channel volume of track 4   &emsp;   (Controller number 0x07, MIDI channel 4)
 *   - Mute button A is the mute button for track 3   &emsp;   (Note number 0x12, MIDI channel 1)
 *   - Mute button B is the mute button for track 4   &emsp;   (Note number 0x13, MIDI channel 1)
 *   - The LED on pin 5 lights up.
 *
 * When bank 3 is selected:
 *   - Potentiometer A is channel volume of track 5   &emsp;   (Controller number 0x07, MIDI channel 5)
 *   - Potentiometer B is channel volume of track 6   &emsp;   (Controller number 0x07, MIDI channel 6)
 *   - Mute button A is the mute button for track 5   &emsp;   (Note number 0x14, MIDI channel 1)
 *   - Mute button B is the mute button for track 6   &emsp;   (Note number 0x15, MIDI channel 1)
 *   - The LED on pin 6 lights up.
 *
 * When bank 4 is selected:
 *   - Potentiometer A is channel volume of track 7   &emsp;   (Controller number 0x07, MIDI channel 7)
 *   - Potentiometer B is channel volume of track 8   &emsp;   (Controller number 0x07, MIDI channel 8)
 *   - Mute button A is the mute button for track 7   &emsp;   (Note number 0x16, MIDI channel 1)
 *   - Mute button B is the mute button for track 8   &emsp;   (Note number 0x17, MIDI channel 1)
 *   - The LED on pin 7 lights up.
 *
 * This allows you to control multiple tracks with only a limited amount of physical potentiometers and buttons
 *
 * Map accordingly in your DAW or DJ software.
 *
 * Written by Pieter P, 08-09-2017  
 * https://github.com/tttapa/Control-Surface
*/

/**
 * @example Ex.06.Banks.ino
 * 
 * This is an example of the Bank class of the Control_Surface library.
 * 
 * Connect two potentiometers to analog pins A0 and A1,
 * and two pushbuttons to pins 2 and 3.
 *
 * Connect a toggle switch to pin 11.
 *
 * When the toggle switch is turned off:
 *  - Potentiometer A is channel volume of track 1    (Controller number 0x07, MIDI channel 1)
 *  - Potentiometer B is channel volume of track 2    (Controller number 0x07, MIDI channel 2)
 *  - Mute button A is the mute button for track 1      (Note number 0x10, MIDI channel 1)
 *  - Mute button B is the mute button for track 2      (Note number 0x11, MIDI channel 1)
 *
 * When the toggle switch is turned on:
 *  - Potentiometer A is channel volume of track 3    (Controller number 0x07, MIDI channel 3)
 *  - Potentiometer B is channel volume of track 4    (Controller number 0x07, MIDI channel 4)
 *  - Mute button A is the mute button for track 3      (Note number 0x12, MIDI channel 1)
 *  - Mute button B is the mute button for track 4      (Note number 0x13, MIDI channel 1)
 *
 * This allows you to control multiple tracks with only a limited amount of physical potentiometers and buttons
 *
 * Map accordingly in your DAW or DJ software.
 *
 * Note: It is recommended to use a BankSelector (See Ex.07.BankSelectors.ino) instead of changing the bank setting manually.
 *
 * ### How it works:
   @par
   When the bank setting is set to 1,
    - 2 (i.e. the number of tracks per bank) will be added to the channel number of the potentiometers.  
      This is because they were added to the bank using the CHANGE_CHANNEL mode.
    - 2 (i.e. the number of tracks per bank) will be added to the note numbers of the buttons.  
      This is because they were added to the bank using the CHANGE_ADDRESS mode;

  
  ### In general:  
  @par
    For controls that were added using the CHANGE_ADDRESS mode:
      - The address will be: startAddress + tracksPerBank * bankSetting
      - The channel won't change.
  @par
    For controls that were added using the CHANGE_CHANNEL mode:
      - The channel will be: startChannel + tracksPerBank * bankSetting
      - The address won't change.

 * Written by Pieter P, 08-09-2017  
 * https://github.com/tttapa/Control-Surface
 */

/**
 * @example Ex.13.Shift-Register.ino
 * 
 * This is an example of the ShiftRegisterOut class of the Control_Surface library.
 * 
 * Connect a potentiometer to analog pins A0.  
 * Connect push buttons to pins 2 and 3, and a shift register to pins 10 (ST_CP), 11 (DS) and 13 (SH_CP).  
 * Connect LEDs (+ current limiting resistors) to the 8 outputs of the shift register.
 *
 * Pins 2 and 3 increment or decrement the bank setting, and the LEDs display the bank setting.
 * 
 * This allows you to control multiple tracks with only a limited amount of physical potentiometers and buttons.
 * 
 * Depending on the bank setting, the potentiometer will send MIDI Control Change 
 * as the channel volume of channels 1 through 8.  
 * Map accordingly in your DAW or DJ software.
 *
 * Written by Pieter P, 08-09-2017  
 * https://github.com/tttapa/Control-Surface
 */

/**
 * @example Ex.02.Button.ino
 * 
 * This is an example of the Digital class of the Control_Surface library.
 * 
 * Connect a push buttons to digital pin 2. Connect the other pin of the button to the ground, 
 * a pull-up resistor is not necessary, because the internal one will be used.  
 * This button will play MIDI note C4 when pressed.  
 * Map it in your DAW or DJ software.
 * 
 * Written by tttapa, 08/09/2017  
 * https://github.com/tttapa/Control-Surface
*/

/**
 * @example Ex.17.Transposer.ino
 * 
 * @brief An example demonstrating the transpose function.
 * 
 * Connect 8 buttons to pins 2 - 9, to use as piano keys 
 * and two buttons to pins 10 - 11 to transpose the notes.
 * 
 * Pressing button 10 once raises the pitch by one semitone,
 * if you press it again, the pitch is two semitones higher, etc.
 * Pressing button 11 lowers the pitch by one semitone at a time.
 * 
 * Written by Pieter P, 27-05-2018   
 * https://github.com/tttapa/Control-Surface
 */

/**
 * @example Ex.14.LED.ino
 * 
 * This is an example of the MIDI_LED class of the Control_Surface librar.
 * 
 * Connect two push buttons to pins 2 and 3. Pull-up resistors are not necessary, 
 * because the internal ones will be used.  
 * Connect two LEDs (+ current limiting resistors) to pins 12 and 13.
 * 
 * The buttons will mute and un-mute tracks 1 and 2 respectively. The LEDs will 
 * indicate whether the channel is muted or not.
 * 
 * Configure as a MCU-compatible Control Surface (Mackie Control Universal) in your DAW.
 * 
 * Written by Pieter P, 2018-04-11  
 * https://github.com/tttapa/Control-Surface
 */

/**
 * @example OLED-Demo.ino
 * 
 * An extensive example demonstrating the use of DisplayElement%s 
 * to display information from the DAW on a small OLED display.
 *        
 * The example displays the cursor time location, play and record status,
 * and for each of the 8 first tracks, it displays:
 * mute, solo, record ready, pan, and VU level meter with peak indicator.
 * 
 * @image html "OLED-Demo.png"
 * 
 * Using a BankSelector, it displays two channels at once, and you can
 * cycle through four banks to display all 8 tracks.
 * 
 * There are two rotary encoders that control the pan of the selected
 * tracks, two mute, solo, record, and select buttons for the tracks, 
 * and a play and record button.
 */

/**
 * @example Ex.03.Rotary-Encoder.ino
 * 
 * This is an example of the RotaryEncoder class of the Control_Surface library.
 * 
 * Connect the A and B pins of the encoder to 2 interrupt pins (2 and 3).  
 * It's recommended to use 100nF capacitors between the A and B pins and ground (hardware debounce).  
 * Connect the common (C) pin to the ground. Pull-up resistors are not necessary, since the internal ones will be used.  
 * Map the control change message 0x14 to the right control in your DJ or DAW software, 
 * and select 'relative' instead of 'absolute'.
 *
 * If you are using a jog wheel, use JOG, if you are using a normal encoder 
 * (for normal controls like EQ or volume etc.) use NORMAL_ENCODER.  
 * If you have strange results in your software, try a different relative mode: 
 * TWOS_COMPLEMENT, BINARY_OFFSET or SIGN_MAGNITUDE.  
 * If the control works, but it goes in the wrong direction, swap the pins A and B of the encoder 
 * (either physically, or by swapping the pin numbers in the RotaryEncoder constructor).
 *
 * Written by Pieter P, 22-08-2017  
 * https://github.com/tttapa/Control-Surface
*/

/**
 * @example Ex.15.LED-Shift-Register.ino
 * 
 * This is an example of the ShiftRegisterOut class of the Control_Surface library.
 *
 * Connect three daisy-chained shift registers to pins 11 (ST_CP), 12 (DS) and 10 (SH_CP).  
 * Connect 8 RGB LEDs (+ current limiting resistors) to the outputs of the shift registers
 * (R-G-B-R-G-B-...).  
 * If blue LEDs light up when red ones should, either flip all the LEDs, or change the 
 * bit order in the ShiftRegisterOut constructor to 'MSBFIRST' instead fo 'LSBFIRST'.
 * 
 * <pre>
 * 10 >──────────────┬─────────────────────────────┬─────────────────────────────┐            
 *        ┏━━━━━━━━━━┷━━━━━━━━━━━┓      ┏━━━━━━━━━━┷━━━━━━━━━━━┓      ┏━━━━━━━━━━┷━━━━━━━━━━━┓
 *        ┃        SH_CP         ┃      ┃        SH_CP         ┃      ┃        SH_CP         ┃
 * 12 >───┨ Data in     Data out ┠──────┨ Data in     Data out ┠──────┨ Data in     Data out ┃
 *        ┃        ST_CP         ┃      ┃        ST_CP         ┃      ┃        ST_CP         ┃
 *        ┗━━━━━━━━━━┯━━━━━━━━━━━┛      ┗━━━━━━━━━━┯━━━━━━━━━━━┛      ┗━━━━━━━━━━┯━━━━━━━━━━━┛
 * 11 >──────────────┴─────────────────────────────┴─────────────────────────────┘            
 * </pre>
 * 
 * The 8 LEDs represent 8 channels. A red LED indicates that the channel is muted, 
 * a green LED indicates that solo is enabled, and a blue LED indicates that the 
 * channel is armed for recording.
 *
 * Configure as a MCU-compatible Control Surface (Mackie Control Universal) in your DAW.
 *
 * Written by Pieter P, 22-03-2018  
 * https://github.com/tttapa/Control-Surface
 */

/**
 * @example Ex.11.Button-Matrix.ino
 * 
 * This is an example of the ButtonMatrix class of the Control_Surface library.
 * 
 * Connect a 4 × 3 matrix of buttons with the rows to pins 2, 3, 4 and 5, 
 * and the columns to pins 6, 7 and 8.  
 * Pull-up resistors are not necessary, because the internal ones will be used. 
 * 
 * If you want to be able to press multiple buttons at once, add a diode 
 * in series with each button, as shown in the following schematic:
 * 
 * @image html Button-matrix.svg
 * 
 * The note numbers are specified in the `addresses` array.  
 * Map accordingly in your DAW or DJ software.
 * 
 * Written by tttapa, 24/09/2017  
 * https://github.com/tttapa/Control-Surface
*/
