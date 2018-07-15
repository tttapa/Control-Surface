#include "Button.h"

using namespace ExtIO;

Button::Button(pin_t pin) : pin(pin) {
    // Enable the internal pull-up resistor on
    // the pin with the button/switch
    ExtIO::pinMode(pin, INPUT_PULLUP);
}

void Button::invert() { invertState = true; }

bool Button::invertState = false;

Button::State Button::getState() {
    State rstate;
    bool state = ExtIO::digitalRead(pin) ^
                 invertState; // read the button state and invert
                              // it if "invertState" is true
    unsigned long now = millis();
    if (now - prevBounceTime > debounceTime) { // wait for state to stabilize
        rstate = static_cast<State>((debouncedState << 1) | state);
        debouncedState = state;
    } else {
        rstate = static_cast<State>(debouncedState << 1 | debouncedState);
    }
    if (state != prevState) { // Button is pressed, released or bounces
        prevBounceTime = now;
        prevState = state;
    }
    return rstate;
}