#include "Button.hpp"

using namespace ExtIO;

Button::Button(pin_t pin) : pin(pin) {}

void Button::begin() { ExtIO::pinMode(pin, INPUT_PULLUP); }

void Button::invert() { invertState = true; }

#ifndef INDIVIDUAL_BUTTON_INVERT
bool Button::invertState = false;
#endif

Button::State Button::getState() {
    State rstate;
    // read the button state and invert it if "invertState" is true
    bool state = ExtIO::digitalRead(pin) ^ invertState;
    unsigned long now = millis();
    if (now - prevBounceTime > debounceTime) { // wait for state to stabilize
        rstate = static_cast<State>((debouncedState << 1) | state);
        debouncedState = state;
    } else {
        rstate = static_cast<State>((debouncedState << 1) | debouncedState);
    }
    if (state != prevState) { // Button is pressed, released or bounces
        prevBounceTime = now;
        prevState = state;
    }
    return rstate;
}

const char *Button::getName(Button::State state) {
    switch (state) {
        case Button::Pressed: return "Pressed";
        case Button::Released: return "Released";
        case Button::Falling: return "Falling";
        case Button::Rising: return "Rising";
        default: return ""; // Keeps the compiler happy
    }
}

unsigned long Button::stableTime() { return millis() - prevBounceTime; }
