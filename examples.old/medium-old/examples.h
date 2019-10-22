/**
 * @example   ExtendedInputOutput/PushButtonsToggleLEDsShiftRegMultiplex/PushButtonsToggleLEDsShiftRegMultiplex.ino
 * 
 * PushButtonsToggleLEDsShiftRegMultiplex
 * ======================================
 *

 * @brief   This example demonstrates the use of push buttons and LEDs and how 
 *          to use shift registers and analog multiplexers to save pins.  
 * <br>  
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
 * @example   ExtendedInputOutput/ShiftRegisterBlink/ShiftRegisterBlink.ino
 * 
 * ShiftRegisterBlink
 * ==================
 *

 * @brief   This example demonstrates the use of shift registers as if they were
 *          just normal IO pins.
 * <br>  
 * Connections:  
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
 * This sketch will just blink the LED once a second.
 * 
 * Written by PieterP, 2018-08-29
 * https://github.com/tttapa/Control-Surface
 
 */

/**
 * @example   ExtendedInputOutput/SPIShiftRegisterBlink/SPIShiftRegisterBlink.ino
 * 
 * SPIShiftRegisterBlink
 * =====================
 *

 * @brief   This example demonstrates the use of shift registers as if they were
 *          just normal IO pins.
 * <br>  
 * Connections:  
 * 
 *   - SS:   74HC595 ST_CP
 *   - MOSI: 74HC595 DS
 *   - CLK:  74HC595 SH_CP
 * 
 * Connect an LED (and series resistor) between the first output of the
 * shift register and ground. 
 * 
 * Remember to connect the enable pin the shift register to ground in order to 
 * enable it.  
 *  
 * This sketch will just blink the LED once a second.
 * 
 * Written by PieterP, 2018-09-01
 * https://github.com/tttapa/Control-Surface
 
 */

/**
 * @example   ExtendedInputOutput/Ex.1.Shift-Register-LED-Effects/Ex.1.Shift-Register-LED-Effects.ino
 * 
 * Ex.1.Shift-Register-LED-Effects
 * ===============================
 *

 * @brief   This is an example of the ShiftRegisterOut class of the Control
 *          Surface library. It creates light effects using three 8-bit shift
 *          registers and some RGB LEDs.
 *<br>  
 * Connect three daisy-chained shift registers to pins 10 (ST_CP), 11 (DS) and 
 * 13 (SH_CP).  
 * Connect 8 RGB LEDs (+ current limiting resistors) to the outputs of the shift
 * registers.
 * 
 * ```
 * 13  >───────────┬──────────────────────┬──────────────────────┐
 *         ┏━━━━━━━┷━━━━━━━┓      ┏━━━━━━━┷━━━━━━━┓      ┏━━━━━━━┷━━━━━━━┓
 *         ┃     SH_CP     ┃      ┃     SH_CP     ┃      ┃     SH_CP     ┃
 * 11  >───┨ DS        Q7S ┠──────┨ DS        Q7S ┠──────┨ DS        Q7S ┃
 *         ┃     ST_CP     ┃      ┃     ST_CP     ┃      ┃     ST_CP     ┃
 *         ┗━━━━━━━┯━━━━━━━┛      ┗━━━━━━━┯━━━━━━━┛      ┗━━━━━━━┯━━━━━━━┛
 * 10  >───────────┴──────────────────────┴──────────────────────┘
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
 * This example will turn on all red LEDs one by one, then turn them off one by
 * one, next, it will turn on and off all green LEDs in the same manner, and
 * finally the same for the blue LEDs. This is repeated indefinitely.
 * 
 * Written by Pieter P, 13-07-2018  
 * https://github.com/tttapa/Control-Surface
 
 */

/**
 * @example   ExtendedInputOutput/Multiplexer-AnalogReadSerial/Multiplexer-AnalogReadSerial.ino
 * 
 * Multiplexer-AnalogReadSerial
 * ============================
 *

 * @brief   This is an example of the AnalogMultiplex class.
 * <br>  
 * ### Connections
 *  - 9:  CD74HC4067 address pin S0
 *  - 10: CD74HC4067 address pin S1
 *  - 11: CD74HC4067 address pin S2
 *  - 12: CD74HC4067 address pin S3
 *  - A0: CD74HC4067 signal pin ("common input/output")
 *  
 * Optionally you can connect the enable pin as well, this is useful
 * if you want to use multiple multiplexers with the same address 
 * lines and the same analog input. Otherwise, just connect the enable
 * pin to ground.
 * 
 * If you are using a 3-bit multiplexer, like the CD74HC4051, you can
 * uncomment the code specific to this multiplexer, and only use three
 * address pins.
 *  
 * ### Behavior
 * Connect some potentiometers or other analog sensors to the 16 inputs
 * of the multiplexer, and open the serial monitor (CTRL+SHIFT+M) or the
 * serial plotter (CTRL+SHIFT+L). You should see all 16 signals printed
 * or plotted.
 * 
 * Written by Pieter P, 31-01-2019  
 * https://github.com/tttapa/Control-Surface
 
 */

/**
 * @example   MIDIInput/LED/LED.ino
 * 
 * LED
 * ===
 *

 * @brief   This example demonstrates the use of LEDs that respond to MIDI input.
 * <br>  
 * If a MIDI Note On event for note 0x3C (middle C) is sent, the built-in LED
 * will light up, if a Note Off event for that note is sent, the LED will turn
 * off.
 * 
 * Written by PieterP, 07-02-2019  
 * https://github.com/tttapa/Control-Surface
 
 */

/**
 * @example   Display/VU-Bridge/VU-Bridge.ino
 * 
 * VU-Bridge
 * =========
 *

 * @brief   This is an example on how to use an OLED display to display the 
 *          VU meters and mute/solo states of the eight first tracks, by using 
 *          the Arduino as a Mackie Control Universal.  
 * <br>  
 * If you want to display more than eight tracks, have a look at the 
 * @ref VU-Bridge-Dual-Display.ino example, that uses MCU Extenders to display 
 * up to 32 tracks.
 * 
 * The OLED display uses a RAM frame buffer of 1 KiB (128×64 pixels / 8 pixels
 * per byte). This is a significant amount for AVR boards like the Arduino UNO,
 * Nano, Leonardo, etc. Keep in mind that you might run out of memory, and that
 * you'll need a more powerful board.
 * 
 * ### Connections
 * This example drives two SSD1306 OLED displays over SPI
 *  - SCK:  SSD1306 D0
 *  - MOSI: SSD1306 D1
 *  - 19:   SSD1306 DC
 *  - 18:   SSD1306 CS
 * 
 * Add a capacitor between the reset pin of the display and ground, and a 
 * resistor from reset to 3.3V. The values are not critical, 0.1µF and 10kΩ 
 * work fine.  
 * You do need some way to reset the display, without it, it won't work.  
 * Alternatively, you could use an IO pin from the Arduino to reset the 
 * display, but this just "wastes" a pin.
 * 
 * @note    Don't forget that most OLED displays are 3.3V only, so connecting 
 *          a display to a 5V Arduino directly will destroy it!
 * 
 * ### Behavior
 * Map "Control Surface" as a Mackie Control Universal unit in your DAW.
 * 
 * The first display should now display the level meters and mute/solo states
 * of the first 8 tracks.
 * 
 * @note    There seem to be some differences in the way some applications 
 *          handle VU meters: some expect the hardware to decay automatically,
 *          some don't.  
 *          If you notice that the meters behave strangely, try both decay 
 *          options of the MCU::VU class, or try a different decay time.
 * 
 * ### Demo
 * 
 * @htmlonly
 * <iframe width="560" height="315"
 * src="https://www.youtube.com/embed/_NJh6LyJjU8?start=25" frameborder="0"
 * allow="accelerometer; autoplay; encrypted-media; gyroscope;
 * picture-in-picture" allowfullscreen></iframe>
 * @endhtmlonly
 
 */

/**
 * @example   Display/OLED-Demo/OLED-Demo.ino
 * 
 * OLED-Demo
 * =========
 *

 * @brief An extensive example demonstrating the use of DisplayElement%s
 *        to display information from the DAW on a small OLED display.
 *<br>  
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
 * @example   Display/VU-Bridge-Dual-Display/VU-Bridge-Dual-Display.ino
 * 
 * VU-Bridge-Dual-Display
 * ======================
 *

 * @brief   This is an example on how to use multiple displays to display the 
 *          VU meters of many tracks, by using the Arduino as a Mackie Control
 *          Universal with extenders.  
 * <br>  
 * This example is currenty only supported using MIDI over USB on Teensy boards,
 * due to limitations of the MIDIUSB library.
 * 
 * ### Connections
 * This example drives two SSD1306 OLED displays over SPI
 *  - SCK:  SSD1306 D0 (both of the displays)
 *  - MOSI: SSD1306 D1 (both of the displays)
 *  - 19:   SSD1306 DC (both of the displays)
 *  - 17:   SSD1306 CS (of the first display)
 *  - 18:   SSD1306 CS (of the second display)
 * 
 * Connect the reset pins of the two displays together, connect a capacitor from 
 * reset to ground, and a resistor from reset to 3.3V. The values are not
 * critical, 0.1µF and 10kΩ work fine.  
 * You do need some way to reset the displays, without it, they won't work.  
 * Alternatively, you could use an IO pin from the Teensy to reset the displays,
 * but this just "wastes" a pin.
 * 
 * @note    Don't forget that most OLED displays are 3.3V only, so connecting 
 *          them to a 5V Arduino directly will destroy them!
 * 
 * ### Behavior
 * Select "MIDIx4" from the Tools > USB Type menu.  
 * Map "Control Surface (1)" as a Mackie Control Universal unit in your DAW, 
 * and map "Control Surface (2)" as a Mackie Control Universal Extender (XT).  
 * If you have to manually set the track offset of the extender, choose 8.
 * 
 * The first display should now display the level meters and mute/solo states
 * of the first 8 tracks, and the second display should display the ones of 
 * tracks 9-16.
 * 
 * @note    There seem to be some differences in the way some applications 
 *          handle VU meters: some expect the hardware to decay automatically,
 *          some don't.  
 *          If you notice that the meters behave strangely, try both decay 
 *          options of the MCU::VU class, or try a different decay time.
 * 
 * Written by PieterP, 09-02-2019  
 * https://github.com/tttapa/Control-Surface
 
 */

/**
 * @example   ESP32/VU-Bridge-ESP32/VU-Bridge-ESP32.ino
 * 
 * VU-Bridge-ESP32
 * ===============
 *

 * @brief   This is an example on how to use an OLED display to display the 
 *          VU meters and mute/solo states of the eight first tracks, by using 
 *          the Arduino as a Mackie Control Universal. This is an example 
 *          modified to work with the ESP32 only, using MIDI over Bluetooth Low 
 *          Energy.  
 * <br>  
 * ### Connections
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
 * ### Behavior
 * Map "Control Surface" as a Mackie Control Universal unit in your DAW.
 * 
 * The first display should now display the level meters and mute/solo states
 * of the first 8 tracks.
 * 
 * @note    There seem to be some differences in the way some applications 
 *          handle VU meters: some expect the hardware to decay automatically,
 *          some don't.  
 *          If you notice that the meters behave strangely, try both decay 
 *          options of the MCU::VU class, or try a different decay time.
 * 
 * ### Demo
 * 
 * @htmlonly
 * <iframe width="560" height="315"
 * src="https://www.youtube.com/embed/_NJh6LyJjU8?start=25" frameborder="0"
 * allow="accelerometer; autoplay; encrypted-media; gyroscope;
 * picture-in-picture" allowfullscreen></iframe>
 * @endhtmlonly
 
 */

/**
 * @example   MIDIOutput/Control-Change/CCRotaryEncoder/CCRotaryEncoder.ino
 * 
 * CCRotaryEncoder
 * ===============
 *
 
 * This is an example that demonstrates the use of a rotary encoder that can 
 * be used for scrolling through a song list, tuning effects, etc.
 *<br>  
 * ### Connections
 * 
 *   - 2: pin A of the rotary encoder
 *   - 3: pin B of the rotary encoder
 * 
 * The internal pull-up resistors will be enabled automatically.  
 * Connect the common (C) pin of the rotary encoder to ground.
 * 
 * ### Behavior
 * 
 * If the encoder is rotated clockwise, a MIDI CC increment message is sent, 
 * if the encoder is rotated counter-clockwise, a MIDI CC decrement message is 
 * sent.  
 * The controller number `MCU::V_POT_1` will be used.  
 * 
 * To change the encoding of the MIDI CC increment and decrement messages, use
 * `RelativeCCSender::setMode`.
 * 
 * Map the Arduino as a Mackie Control Universal (MCU) or equivalent in your
 * audio software.
 * 
 * Written by PieterP, 2019-02-16  
 * https://github.com/tttapa/Control-Surface
 
 */

/**
 * @example   MIDIOutput/Control-Change/CCPotentiometer/CCPotentiometer.ino
 * 
 * CCPotentiometer
 * ===============
 *

 * @brief   This is an example of the `CCPotentiometer` class of the 
 *          Control_Surface library.
 * <br>  
 * Connect a potentiometer to analog pin A0. This will be the MIDI channel volume of channel 1.  
 * Map it in your DAW or DJ software.
 * 
 * Written by Pieter P, 28-08-2018  
 * https://github.com/tttapa/Control-Surface
 
 */

/**
 * @example   MIDIOutput/Control-Change/CCPotentiometer-Map/CCPotentiometer-Map.ino
 * 
 * CCPotentiometer-Map
 * ===================
 *

 * @brief   This is an example of the `CCPotentiometer` class of the 
 *          Control_Surface library, and specifically, how to use the `map`
 *          function to account for non-linearities of the potentiometer.
 * <br>  
 * Connect a potentiometer to analog pin A0. This will be the MIDI channel 
 * volume of channel 1.  
 * Map it in your DAW or DJ software.
 * 
 * Written by Pieter P, 16-06-2019  
 * https://github.com/tttapa/Control-Surface
 
 */

/**
 * @example   MIDIOutput/Control-Change/CCIncrementDecrementButtons/CCIncrementDecrementButtons.ino
 * 
 * CCIncrementDecrementButtons
 * ===========================
 *
 
 * This is an example that demonstrates the use of Increment and 
 * Decrement buttons that can be used for scrolling through a song
 * list, tuning effects, etc.  
 * You can compare it to a rotary encoder, but with two buttons instead of an 
 * encoder.
 *<br>  
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
 * @example   MIDIOutput/Notes/PushButtons/PushButtons.ino
 * 
 * PushButtons
 * ===========
 *

 * @brief   This example demonstrates the use of push buttons that send note 
 *          events.
 * <br>  
 * ### Connections
 * 
 *  - 2:  momentary push button (to ground)
 *  - 3:  momentary push button (to ground)
 * 
 * The internal pull-up resistors for the buttons will be enabled automatically.
 * 
 * ### Behavior
 * 
 * Pressing the first button will send note on/off events on middle C.
 * Pressing the second button will send note on/off events for middle C sharp.
 * 
 * Written by PieterP, 28-01-2019  
 * https://github.com/tttapa/Control-Surface
 
 */

/**
 * @example   MIDIOutput/Notes/PushButtonsLEDs/PushButtonsLEDs.ino
 * 
 * PushButtonsLEDs
 * ===============
 *

 * @brief   This example demonstrates the use of push buttons and LEDs.
 * <br>  
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
 * @example   MIDIOutput/Notes/PushButtonsLEDsNoMIDIFeedback/PushButtonsLEDsNoMIDIFeedback.ino
 * 
 * PushButtonsLEDsNoMIDIFeedback
 * =============================
 *

 * @brief   This example demonstrates the use of push buttons and LEDs without
 *          feedback from the software.
 *<br>  
 * This means that the state of the LEDs could be wrong if you click the button
 * on your computer.
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
 * Written by PieterP, 2019-01-23
 * https://github.com/tttapa/Control-Surface
 
 */

/**
 * @example   MIDIOutput/Notes/NoteButtonMatrixKeyboard/NoteButtonMatrixKeyboard.ino
 * 
 * NoteButtonMatrixKeyboard
 * ========================
 *
 
 * @brief   This is an example to demonstrate the use of a ButtonMatrix.
 * <br>  
 * ### Connections
 *  
 *  - A0-A3: The row pins of the button matrix (with the cathodes of the diodes)
 *  - 2-13:  The column pins of the button matrix
 *  - A4-A5: Two push buttons (to ground) to transpose the keyboard
 * 
 * The internal pull-up resistors will be used.
 * 
 * ### Behavior
 * 
 * The keys of the keyboard should send note on/off events when 
 * pressed/released.  
 * The notes can be transposed by pressing the buttons connected to pins A4-A5.
 * 
 * Written by Pieter P, 27-01-2019  
 * https://github.com/tttapa/Control-Surface
 
 */

/**
 * @example   MIDIOutput/Notes/PushButtonsLEDsShiftRegMultiplex/PushButtonsLEDsShiftRegMultiplex.ino
 * 
 * PushButtonsLEDsShiftRegMultiplex
 * ================================
 *

 * @brief   This example demonstrates the use of push buttons and LEDs and how 
 *          to use shift registers and analog multiplexers to save pins.  
 * <br>  
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
 * @example   ProgramChange/ProgramChanger/ProgramChanger.ino
 * 
 * ProgramChanger
 * ==============
 *

 * @brief   This is an example of the ProgramChanger and Selector classes.
 * <br>  
 * ### Connections
 *  - 5: A momentary push button (to ground)
 *  - 6: A momentary push button (to ground)
 *  
 * The internal pull-up resistors will be enabled automatically
 *  
 * ### Behavior
 * When you press the first button, the next program is selected.  
 * When you press the second button, the previous program is selected.  
 * When you press both buttons at once, it resets to the first program
 * again.  
 * If you keep pressin one of the buttons, it will cycle through the
 * programs more quickly.
 * 
 * Written by Pieter P, 30-01-2019  
 * https://github.com/tttapa/Control-Surface
 
 */

/**
 * @example   Audio/OLED-Demo-Audio/OLED-Demo-Audio.ino
 * 
 * OLED-Demo-Audio
 * ===============
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
 * @example   Audio/VUMeterOLED-USB-DAC/VUMeterOLED-USB-DAC.ino
 * 
 * VUMeterOLED-USB-DAC
 * ===================
 *

 * Connections:
 * <br>  
 * A0: wiper of a potentiometer to change the output volume
 * A1: wiper of a potentiometer to change the VU gain
 * 
 * 9:  BCK (I²S)
 * 11: SCK (I²S)
 * 22: DIN (I²S)
 * 23: LRCK (I²S)
 
 */

/**
 * @example   Audio/VUMeterLEDs-USB-DAC/VUMeterLEDs-USB-DAC.ino
 * 
 * VUMeterLEDs-USB-DAC
 * ===================
 *

 * Connections:
 *<br>  
 * A0: wiper of a potentiometer to change the output volume
 * A1: wiper of a potentiometer to change the VU gain
 *
 * 9:  BCK (I²S)
 * 11: SCK (I²S)
 * 22: DIN (I²S)
 * 23: LRCK (I²S)

 */

/**
 * @example   MIDI_Interfaces/Bluetooth-BLE-MIDI-NoteButton/Bluetooth-BLE-MIDI-NoteButton.ino
 * 
 * Bluetooth-BLE-MIDI-NoteButton
 * =============================
 *

 * @brief   This is an example of the `BluetoothMIDI_Interface` class of the 
 *          Control_Surface library. (ESP32 only)
 * <br>  
 * Connect a momentary push button to pin 18.  
 * Pressing it will send middle C MIDI Note events.
 * 
 * On Android, I used the MIDI BLE Connect and MIDI Scope apps by Mobileer Inc
 * to test it.
 *  
 * Written by Pieter P, 26-06-2019  
 * https://github.com/tttapa/Control-Surface
 
 */

/**
 * @example   MIDI_Interfaces/Bluetooth-BLE-MIDI-NoteButton_Input/Bluetooth-BLE-MIDI-NoteButton_Input.ino
 * 
 * Bluetooth-BLE-MIDI-NoteButton_Input
 * ===================================
 *

 * @brief   This is an example of the `BluetoothMIDI_Interface` class of the 
 *          Control_Surface library. (ESP32 only)
 * <br>  
 * Connect a momentary push button to pin 18.  
 * Pressing it will send middle C MIDI Note events.  
 * Pressing the middle C key on the computer/phone turns on 
 * the built-in LED of the ESP32.
 * 
 * On Android, I used the MIDI BLE Connect (Mobileer Inc)
 * and MIDI Keyboard (Dreamhound Studios) apps to test it.
 * 
 * Written by Pieter P, 26-06-2019  
 * https://github.com/tttapa/Control-Surface
 
 */

/**
 * @example   MIDI_Interfaces/Bluetooth-BLE-MIDI-CCPotentiometer/Bluetooth-BLE-MIDI-CCPotentiometer.ino
 * 
 * Bluetooth-BLE-MIDI-CCPotentiometer
 * ==================================
 *

 * @brief   This is an example of the `BluetoothMIDI_Interface` class of the 
 *          Control_Surface library. (ESP32 only)
 * <br>  
 * Connect a potentiometer to analog pin 36. This will be the MIDI channel 
 * volume of channel 1.  
 * Map it in your DAW or DJ software.
 * 
 * On Android, I used the MIDI BLE Connect and MIDI Scope apps by Mobileer Inc
 * to test it.
 * 
 * Written by Pieter P, 26-06-2019  
 * https://github.com/tttapa/Control-Surface
 
 */

/**
 * @example   Getting-Started/Getting-Started.ino
 * 
 * Getting-Started
 * ===============
 *

 * This is the example used in the 'Getting Started' guide.  
 * Read the guide: https://github.com/tttapa/Control-Surface/blob/revision-4/doc/Getting-Started.md
 * <br>  
 * Written by PieterP, 29-08-2018  
 * https://github.com/tttapa/Control-Surface
 
 */

