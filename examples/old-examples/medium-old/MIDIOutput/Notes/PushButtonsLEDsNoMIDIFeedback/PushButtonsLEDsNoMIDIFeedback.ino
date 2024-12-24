/**
 * @brief   This example demonstrates the use of push buttons and LEDs without
 *          feedback from the software.
 *
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

// Include the Control Surface library
#include <Control_Surface.h>

// Instantiate a MIDI over USB interface.
USBMIDI_Interface midi;

// Instantiate the momentary pbuush buttons that send out MIDI Note events.
NoteButtonLatched buttons[] = {
    {2, MCU::MUTE_1}, // digital input pin, note number
    {3, MCU::MUTE_2},
};

constexpr size_t numberOfButtons = sizeof(buttons) / sizeof(*buttons);

// The pins of the LEDs that will display the state of the buttons
pin_t ledPins[] = {
    12,
    13,
};

constexpr size_t numberOfLEDs = sizeof(ledPins) / sizeof(*ledPins);
static_assert(numberOfButtons == numberOfLEDs,
              "Error: number of buttons is not "
              "the same as the number of LEDs!");
void setup() {
    // Initialize everything
    Control_Surface.begin();

    for (pin_t &pin : ledPins)
        pinMode(pin, OUTPUT);
}

void loop() {
    // Update the control surface: read new MIDI events, and read all inputs
    Control_Surface.loop();

    for (uint8_t i = 0; i < numberOfLEDs; ++i)
        digitalWrite(ledPins[i], buttons[i].getState());
}
