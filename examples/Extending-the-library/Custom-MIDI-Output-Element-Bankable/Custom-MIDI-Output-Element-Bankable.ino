/** 
 * This is an example that demonstrates how to extend the library using your own
 * Bankable MIDI Output Elements. 
 * The example declares a MIDI Output Element that sends MIDI Note events when a
 * push button is pressed or released. It can be banked to change the MIDI 
 * address. It's a simplified version of the @ref Bankable::NoteButton class.
 * 
 * To understand this example, you need to understand the @ref Bank.ino and the
 * @ref Custom-MIDI-Output-Element.ino examples first.
 *
 * @boards  AVR, AVR USB, Due, Nano 33 IoT, Nano 33 BLE, UNO R4, Pi Pico, Teensy 3.x, ESP32
 * 
 * Connections
 * -----------
 * 
 * - 5: momentary push button (to ground) to send MIDI notes
 * - 6: momentary push button (to ground) to select the next bank
 * 
 * The internal pull-up resistors for the buttons will be enabled automatically.
 * 
 * Behavior
 * --------
 * 
 * - When the button on pin 5 is pressed, a MIDI Note On message is sent for
 *   note C in the octave determined by the bank setting.
 * - When the button on pin 5 is released, a MIDI Note Off message is sent for 
 *   note C in the octave determined by the bank setting.
 * - When the button on pin 6 is pressed, the bank setting is incremented.
 * 
 * Mapping
 * -------
 * 
 * Select the Arduino as a custom MIDI controller in your DAW, and use the 
 * MIDI learn option to assign the button to a function.
 * 
 * Written by PieterP, 2020-09-29  
 * https://github.com/tttapa/Control-Surface
 */
#include <Control_Surface.h>

BEGIN_CS_NAMESPACE

/*
 * @brief   A class for momentary buttons and switches that send MIDI events.
 *
 * The button is debounced, and the internal pull-up resistor is enabled.
 *
 * @see     Bankable::NoteButton
 * @see     Button
 */
class MyNoteButton : public MIDIOutputElement {
 public:
  /*
   * @brief   Create a new MyNoteButton object on the given pin, with the 
   *          given address and velocity.
   * 
   * @param   bankConfig
   *          Specifies the bank that determines the address of this button, as
   *          well as the bank type. The bank type determines whether to change
   *          the address (note number), the MIDI channel, or the MIDI USB 
   *          cable number.
   * @param   pin
   *          The digital input pin to read from.  
   *          The internal pull-up resistor will be enabled.
   * @param   address
   *          The MIDI address to send to.
   * @param   velocity
   *          The MIDI note velocity [0, 127].
   */
  MyNoteButton(OutputBankConfig<> bankConfig, pin_t pin,
               MIDIAddress baseAddress, uint8_t velocity)
    : address(bankConfig, baseAddress), button(pin), velocity(velocity) {}

 public:
  // Initialize: enable the pull-up resistor for the button.
  // This method is called once by `Control_Surface.begin()`.
  void begin() final override { button.begin(); }

  // Update: read the button and send MIDI messages when appropriate.
  // This method is called continuously by `Control_Surface.loop()`.
  void update() final override {
    AH::Button::State state = button.update(); // Read the button
    if (state == AH::Button::Falling) {        // if pressed
      // Don't allow changing the bank setting as long as the button is pressed:
      address.lock();
      Control_Surface.sendNoteOn(address.getActiveAddress(), velocity);
    } else if (state == AH::Button::Rising) { // if released
      Control_Surface.sendNoteOff(address.getActiveAddress(), velocity);
      // Button is released, so the bank setting can be changed again.
      address.unlock();
    }
  }

 private:
  Bankable::SingleAddress address;
  AH::Button button;
  uint8_t velocity;
};

/*
 * Compare this class with the class in the Custom-MIDI-Output-Element.ino 
 * example.
 * 
 * You'll notice that the constructor argument `bankConfig` was added to specify
 * the bank to use.
 *  
 * The `address` member variable is no longer of type `MIDIAddress`, it's now of
 * type `Bankable::SingleAddress`. This is a bank-dependent address. The actual
 * `MIDIAddress` that's selected by the bank can be retrieved by calling
 * `address.getActiveAddress()`.
 * 
 * Knowing this, you should be able to understand most of the `update` method,
 * it's almost the same as the Custom-MIDI-Output-Element.ino example, except 
 * for the calls to `lock()` and `unlock()`.
 * 
 * To understand why these are necessary, consider what happens in the following
 * example without locking:
 * 
 * 1. The first bank is selected. This means that `address.getActiveAddress()`
 *    will return note C2.
 * 2. The user presses the button, a Note On event is sent for note C2.
 * 3. The button is still pressed, and the user selects the second bank:
 *    `address.getActiveAddress()` will now return note C3.
 * 4. The user releases the button, a Note Off event is sent for note C3.
 *    This is bad! The note C2 will keep on playing indefinitely, because we 
 *    never sent a Note Off event for that note. 
 * 
 * To fix this, we have to make sure that the Note Off events we send are always
 * sent to the same address as the previous Note On event. The address cannot
 * change in between Note On and Note Off events.
 * That's exactly what the `address.lock()` method does: it freezes the address.
 * After locking, `address.getActiveAddress()` will always return the same
 * address, until `address.unlock()` is called. When the address is unlocked,
 * `address.getActiveAddress()` will just return the address determined by the
 * selected bank.
 */

END_CS_NAMESPACE

// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: //

// Instantiate a MIDI over USB interface.
USBMIDI_Interface midi;

// Instantiate four Banks, with twelve tracks per bank (12 semitones = 1 octave).
Bank<4> bank(12);
// Instantiate a Bank selector to control which one of the four Banks is active.
IncrementSelector<4> selector {
  bank, // Bank to manage
  6,    // push button pin
};

// Instantiate a MyNoteButton object
MyNoteButton button {
  {bank, BankType::ChangeAddress}, // bank changes the note number (address)
  5,                               // Push button on pin 5
  {MIDI_Notes::C[2], Channel_1},   // Base address: Note C2 on MIDI channel 1
  0x7F,                            // Maximum velocity
};

void setup() {
  Control_Surface.begin(); // Initialize Control Surface (calls button.begin())
}

void loop() {
  Control_Surface.loop(); // Update the Control Surface (calls button.update())
}