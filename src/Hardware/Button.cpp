#include "Button.h"

using namespace ExtIO;

Button::Button(pin_t pin) // Constructor
    : pin(pin) {
  ExtIO::pinMode(pin, INPUT_PULLUP); // Enable the internal pull-up resistor on
                                     // the pin with the button/switch
}

void Button::invert() { // Invert the button state (send Note On event when
                        // released, Note Off when pressed)
  invertState = true;
}

bool Button::invertState = false;

Button::State Button::getState() // Check if the button state changed
{
  State rstate;
  bool state =
      ExtIO::digitalRead(pin) ^ invertState; // read the button state and invert
                                             // it if "invertState" is true
  if (millis() - prevBounceTime > debounceTime) { // wait for state to stabilize
    rstate = static_cast<State>((buttonState << 1) | state);
    buttonState = state;
  } else {
    rstate = static_cast<State>(buttonState << 1 | buttonState);
  }
  if (state != prevState) { // Button is pressed, released or bounces
    prevBounceTime = millis();
    prevState = state;
  }
  return rstate;
}