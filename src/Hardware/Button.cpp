#include "Button.h"

using namespace ExtIO;

Button::Button(pin_t pin) : pin(pin) {
  ExtIO::pinMode(pin, INPUT_PULLUP); // Enable the internal pull-up resistor on
                                     // the pin with the button/switch
}

void Button::invert() { invertState = true; }

bool Button::invertState = false;

Button::State Button::getState() {
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