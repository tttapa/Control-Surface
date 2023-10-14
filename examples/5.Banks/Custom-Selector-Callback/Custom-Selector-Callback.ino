/**
 * This example demonstrates how to use custom callbacks to display the value
 * of a selector.
 * 
 * @boards  AVR, AVR USB, Nano 33 IoT, Nano 33 BLE, UNO R4, Pi Pico, Due, Teensy 3.x, ESP8266, ESP32
 * 
 * Connections
 * -----------
 * 
 *   - 10:   Red pin of an RGB LED (+ series resistor)
 *   - 11:   Green pin of an RGB LED (+ series resistor)
 *   - 12:   Blue pin of an RGB LED (+ series resistor)
 *   
 *   - 2:    Push button (to ground, internal pull-up resistor will be used)
 *   - 3:    Push button (to ground, internal pull-up resistor will be used)
 * 
 * Connect the common cathode of the RGB LED to ground.
 * 
 * Behavior
 * --------
 * 
 * The LEDs display the current setting of the bank. 
 * Red is the first bank (0), yellow is the second bank (1), green is the third 
 * bank (2), cyan is the fourth bank (3), blue is the fifth bank (4), and purple
 * is the sixth bank (5).
 * 
 * Use the two push buttons to change the setting. The one on on pin 2 
 * increments the setting, the one on pin 3 decrements the setting.  
 * Keeping one of the buttons pressed will keep on cycling through the settings
 * until you let go of it.  
 * Pressing both buttons simultaneously resets the setting to its initial value.
 * 
 * **Note**: this example doesn't send any MIDI, but you could use it as a 
 * template for a MIDI controller with banks.
 * 
 * Mapping
 * -------
 * 
 * None.
 * 
 * Written by PieterP, 2020-02-03  
 * https://github.com/tttapa/Control-Surface
 */
#include <Control_Surface.h>

// Custom callback to handle output for a selector.
class MySelectorCallback {
 public:
  // Constructor
  MySelectorCallback(pin_t redLED, pin_t greenLED, pin_t blueLED)
    : redLED(redLED), greenLED(greenLED), blueLED(blueLED) {}

  // Begin function is called once by Control Surface.
  // Use it to initialize everything.
  void begin() {
    pinMode(redLED, OUTPUT);
    pinMode(greenLED, OUTPUT);
    pinMode(blueLED, OUTPUT);
    show(0);
  }

  // Update function is called continuously by Control Surface.
  // Use it to implement things like fading, blinking ...
  void update() {}

  // Update function with arguments is called when the setting
  // changes.
  // Use it to update the LEDs.
  void update(setting_t oldSetting, setting_t newSetting) {
    (void)oldSetting; // unused in this example
    show(newSetting);
  }

 private:
  // Show the color of the given setting.
  void show(setting_t setting) {
    uint8_t color = getColor(setting);
    digitalWrite(redLED, color & 0b001 ? HIGH : LOW);
    digitalWrite(greenLED, color & 0b010 ? HIGH : LOW);
    digitalWrite(blueLED, color & 0b100 ? HIGH : LOW);
  }

  // Convert the given setting to a 3-bit RGB color value.
  static uint8_t getColor(setting_t setting) {
    switch (setting) {
      case 0: return 0b001;
      case 1: return 0b011;
      case 2: return 0b010;
      case 3: return 0b110;
      case 4: return 0b100;
      case 5: return 0b101;
      default: return 0b000;
    }
  }

 private:
  // Member variables to remember the pin numbers of the LEDs.
  pin_t redLED, greenLED, blueLED;
};

// :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: //

USBMIDI_Interface midi;

// Create 6 banks with one track per bank.
Bank<6> bank = 1;

// Create a selector that uses our custom callback, to control the bank.
GenericIncrementDecrementSelector<6, MySelectorCallback> selector {
  bank,         // bank to manage
  {10, 11, 12}, // red, green, blue LED pins
                // (this is the MySelectorCallback constructor defined above)
  {2, 3},       // incr/decr button pins
  Wrap::Wrap,   // wrap around when reaching setting 6
};

void setup() {
  Control_Surface.begin(); // initialize
}

void loop() {
  Control_Surface.loop(); // update
}
