/** 
 * This is an example that demonstrates how to extend the library using your own
 * MIDI Output Elements. The example declares a MIDI Output Element that sends 
 * MIDI Note events when a push button is pressed or released. It's a simplified
 * version of the @ref NoteButton class.
 *
 * @boards  AVR, AVR USB, Due, Nano 33 IoT, Nano 33 BLE, UNO R4, Pi Pico, Teensy 3.x, ESP32
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
 * Written by PieterP, 2020-02-04  
 * https://github.com/tttapa/Control-Surface
 */
#include <Control_Surface.h>

BEGIN_CS_NAMESPACE

/*
 * @brief   A class for momentary buttons and switches that send MIDI events.
 *
 * The button is debounced, and the internal pull-up resistor is enabled.
 *
 * @see     NoteButton
 * @see     Button
 */
class MyNoteButton : public MIDIOutputElement {
 public:
  /*
   * @brief   Create a new MyNoteButton object on the given pin, with the 
   *          given address and velocity.
   * 
   * @param   pin
   *          The digital input pin to read from.  
   *          The internal pull-up resistor will be enabled.
   * @param   address
   *          The MIDI address to send to.
   * @param   velocity
   *          The MIDI note velocity [0, 127].
   */
  MyNoteButton(pin_t pin, MIDIAddress address, uint8_t velocity)
    : button(pin), address(address), velocity(velocity) {}

 public:
  // Initialize: enable the pull-up resistor for the button
  // This method is called once by `Control_Surface.begin()`.
  void begin() final override { button.begin(); }

  // Update: read the button and send MIDI messages when appropriate.
  // This method is called continuously by `Control_Surface.loop()`.
  void update() final override {
    AH::Button::State state = button.update();        // Read the button
    if (state == AH::Button::Falling) {               // if pressed
      Control_Surface.sendNoteOn(address, velocity);  //   → note on
    } else if (state == AH::Button::Rising) {         // if released
      Control_Surface.sendNoteOff(address, velocity); //   → note off
    }
  }

 private:
  AH::Button button;
  const MIDIAddress address;
  uint8_t velocity;
};

END_CS_NAMESPACE

// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: //

// Instantiate a MIDI over USB interface.
USBMIDI_Interface midi;

// Instantiate a MyNoteButton object
MyNoteButton button {
  5,                             // Push button on pin 5
  {MIDI_Notes::C[4], Channel_1}, // Note C4 on MIDI channel 1
  0x7F,                          // Maximum velocity
};

void setup() {
  Control_Surface.begin(); // Initialize Control Surface (calls button.begin())
}

void loop() {
  Control_Surface.loop(); // Update the Control Surface (calls button.update())
}