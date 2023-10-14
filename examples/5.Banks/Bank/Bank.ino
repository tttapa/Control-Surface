/** 
 * This is an example that demonstrates the use of Banks.
 *
 * @boards  AVR, AVR USB, Due, Nano 33 IoT, Nano 33 BLE, UNO R4, Pi Pico, Teensy 3.x
 * 
 * Banks allow you to use a single control for many different actions. 
 * By changing the bank setting, you can change the address of a MIDI Element.
 * 
 * In this example, we will use two potentiometers to control the volume of 
 * eight MIDI channels by using four banks.
 * 
 * The setup is as follows:
 * 
 *     ═══════════   ─╮             ═════════════════
 *      Channel 1     │              Potentiometer 1
 *     ──────────     │  Bank 1      ────────────────
 *      Channel 2     │              Potentiometer 2
 *     ═══════════   ─┤             ═════════════════
 *      Channel 3     │              Potentiometer 1
 *     ──────────     │  Bank 2      ────────────────
 *      Channel 4     │              Potentiometer 2
 *     ═══════════   ─┤             ═════════════════
 *      Channel 5     │              Potentiometer 1
 *     ──────────     │  Bank 3      ────────────────
 *      Channel 6     │              Potentiometer 2
 *     ═══════════   ─┤             ═════════════════
 *      Channel 7     │              Potentiometer 1
 *     ──────────     │  Bank 4      ────────────────
 *      Channel 8     │              Potentiometer 2
 *     ═══════════   ─╯             ═════════════════
 * 
 * When Bank 1 is selected, Potentiometer 1 controls Channel 1, and 
 * Potentiometer 2 controls Channel 2.  
 * When Bank 2 is selected, Potentiometer 1 controls Channel 3, and 
 * Potentiometer 2 controls Channel 4.  
 * When Bank 3 is selected, Potentiometer 1 controls Channel 5, and 
 * Potentiometer 2 controls Channel 6.  
 * When Bank 4 is selected, Potentiometer 1 controls Channel 7, and 
 * Potentiometer 2 controls Channel 8.  
 * 
 * To select the active bank, you can either use the @ref Bank::select method,
 * or you can use one of the many @ref Selectors.  
 * In this example, we'll use an @ref IncrementDecrementSelector. It has one
 * push button to increment the bank setting, and one push button to decrement
 * the bank setting.
 * 
 * To display which bank is currently active, you can use the Selectors with LED
 * feedback in the @ref SelectorsLEDs module, or you can write your own callback
 * that gets called when the setting changes. The latter is demonstrated in the
 * @ref Custom-Selector-Callback.ino example.
 * 
 * For more information about banks, have a look at the 
 * @ref faq-banks "“How do banks work?”" FAQ page, it includes a nice animation.
 * 
 * Connections
 * -----------
 * 
 * - 2: momentary push button (to ground)
 * - 3: momentary push button (to ground)
 * - A0: wiper of a potentiometer
 * - A1: wiper of a potentiometer
 * 
 * Connect the left terminal of the potentiometer to ground, and the right one
 * to V<sub>CC</sub>.  
 * The internal pull-up resistors for the buttons will be enabled automatically.
 * 
 * Behavior
 * --------
 * 
 * - Press the Bank selector buttons while turning the potentiometers. Notice
 *   how the channel of the MIDI messages changes when you change the Bank, as 
 *   explained above.
 * 
 * Written by PieterP, 2020-02-04  
 * https://github.com/tttapa/Control-Surface
 */

#include <Control_Surface.h> // Include the Control Surface library

// Instantiate a MIDI over USB interface
USBMIDI_Interface midi;

// Instantiate four Banks, with two tracks per bank.
// Compare these numbers to the diagram above.
Bank<4> bank(2);
//   │       └───── number of tracks per bank
//   └───────────── number of banks

// Instantiate a Bank selector to control which one of the four Banks is active.
IncrementDecrementSelector<4> selector {
  bank,       // Bank to manage
  {2, 3},     // push button pins (increment, decrement)
  Wrap::Wrap, // Wrap around
};

// Wrapping around means that if you're in Bank 4 and you press the increment
// button, you wrap back around to Bank 1. Similarly, if you're in Bank 1 and
// you press the decrement button, you wrap back around to Bank 4.
// The alternative to Wrap::Wrap is Wrap::Clamp. In that case, pressing the
// increment button when you're in Bank 4 won't do anything, you'll stay in
// Bank 4. If you're in Bank 1 and press the decrement button, you'll stay in
// Bank 1.

// Instantiate two potentiometers for the volume controls.
Bankable::CCPotentiometer potentiometer1 {
  {bank, BankType::ChangeChannel},      // bank configuration
  A0,                                   // analog pin
  {MIDI_CC::Channel_Volume, Channel_1}, // address
};
Bankable::CCPotentiometer potentiometer2 {
  {bank, BankType::ChangeChannel},      // bank configuration
  A1,                                   // analog pin
  {MIDI_CC::Channel_Volume, Channel_2}, // address
};

// The addresses specified here are the addresses for the first Bank.
// The addresses used in the other Banks are derived from these base addresses
// by adding the number of tracks of the bank to the base address.
// For example, in Bank 1, the first potentiometer will send on Channel 1,
// in Bank 2, the first potentiometer will send on Channel 1 + 2 = Channel 3,
// because 2 is the number of tracks in the Bank.
//
// The bank setting can affect the Channel (as in this example), the address, or
// the MIDI USB Cable Number. You can change this behavior by changing the
// BankType in the bank configuration above to `BankType::ChangeAddress` or
// `BankType::ChangeCable`.

void setup() {
  Control_Surface.begin(); // Initialize Control Surface
}

void loop() {
  Control_Surface.loop(); // Update the Control Surface
}
