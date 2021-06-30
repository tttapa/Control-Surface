/** 
 * This is an example that demonstrates how to extend the library using your own
 * MIDI Senders. It implements functionality similar to the built-in 
 * @ref DigitalNoteSender class, but with support for different on and off 
 * velocities.
 * 
 * @see @ref MIDI_Senders for different kinds of MIDI senders to start from.
 *
 * @boards  AVR, AVR USB, Due, Nano 33 IoT, Nano 33 BLE, Teensy 3.x, ESP32
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
 *   note C4 with velocity 0x40.
 * - When the button on pin 5 is released, a MIDI Note Off message is sent for 
 *   note C4 with velocity 0x10.
 * 
 * Mapping
 * -------
 * 
 * Select the Arduino as a custom MIDI controller in your DAW, and use the 
 * MIDI learn option to assign the button to a function.
 * 
 * Written by PieterP, 2020-04-18  
 * https://github.com/tttapa/Control-Surface
 */
#include <Control_Surface.h>

USBMIDI_Interface midi;

// A custom sender to use later. It has to declare two methods:
// - sendOn(MIDIAddress): will be called when the button is pressed
// - sendOff(MIDIAddress): will be called when the button is released
// Other types of senders need to implement different methods.
// For example, a continuous CC sender just has a send(value, address)
// method.
class CustomNoteSender {
 public:
  CustomNoteSender(uint8_t onVelocity, uint8_t offVelocity)
    : onVelocity(onVelocity), offVelocity(offVelocity) {}

  void sendOn(MIDIAddress address) {
    Control_Surface.sendNoteOn(address, onVelocity);
  }

  void sendOff(MIDIAddress address) {
    Control_Surface.sendNoteOff(address, offVelocity);
  }

 private:
  uint8_t onVelocity, offVelocity;
};

// Now tell the MIDIButton class template (included with the Control
// Surface library) that it has to use your custom sender class.
//
// We wrap it in another class so we can easily construct it later,
// without having to write `MIDIButton<CustomNoteSender>` all the time,
// and so we have more control over the constructor arguments.
// The colon (:) indicates inheritance.
struct CustomNoteButton : MIDIButton<CustomNoteSender> {
  // Constructor
  CustomNoteButton(pin_t pin, MIDIAddress address, uint8_t onVelocity,
                   uint8_t offVelocity)
    : MIDIButton(pin, address, {onVelocity, offVelocity}) {}
  //  ^~~~~~~~~~ Initialization of the base class MIDIButton
};

// Now we can instantiate an object of our custom class.
// The four arguments match the ones of the CustomNoteButton
// constructor we wrote a couple of lines back.
CustomNoteButton button {
  5,              // button pin
  MIDI_Notes::C(4), // MIDI address
  0x40,           // on velocity
  0x10,           // off velocity
};

void setup() {
  Control_Surface.begin();
}

void loop() {
  Control_Surface.loop();
}
