#ifndef CONTROL_SURFACE_MIDI_OUTPUTS_BUTTON_H_
#define CONTROL_SURFACE_MIDI_OUTPUTS_BUTTON_H_

#include "../Settings/SettingsWrapper.h"
#include "../ExtendedInputOutput/ExtendedInputOutput.h"

class Button
{
public:
  /**
   * @brief Construct a new Button object.
   * 
   * @param pin The digital pin to read from. The internal pull-up resistor will be enabled.
   */
  Button(pin_t pin); // Constructor

  /**
   * @brief Invert the state of all buttons (button pressed is HIGH instead of LOW).
   * 
   * @note This impacts **all** Button objects.
   */
  static void invert();

  bool isPressed() {
    return buttonState;
  }

protected:
  virtual void press() = 0;
  virtual void release() = 0;

private:
  void refresh(); // Check if the button state changed, and send a MIDI Note On or Off accordingly

  const pin_t pin;
  bool prevState = HIGH;
  bool buttonState = HIGH;
  unsigned long prevBounceTime = 0;

  static bool invertState;

  constexpr static unsigned long debounceTime = BUTTON_DEBOUNCE_TIME; // Edit this in ../Settings/Settings.h

  constexpr static int8_t falling = LOW - HIGH;
  constexpr static int8_t rising = HIGH - LOW;
};

#endif // CONTROL_SURFACE_MIDI_OUTPUTS_BUTTON_H_