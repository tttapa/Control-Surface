#include "Button.h"

using namespace ExtIO;

Button::Button(pin_t pin) // Constructor
    : pin(pin)
{
    ExtIO::pinMode(pin, INPUT_PULLUP); // Enable the internal pull-up resistor on the pin with the button/switch
}

void Button::invert() // Invert the button state (send Note On event when released, Note Off when pressed)
{
    invertState = true;
}
bool Button::invertState = false;

void Button::refresh() // Check if the button state changed
{
    bool state = ExtIO::digitalRead(pin) ^ invertState; // read the button state and invert it if "invertState" is true
    if (millis() - prevBounceTime > debounceTime)
    {
        int8_t stateChange = state - buttonState;
        buttonState = state;
        if (stateChange == falling) // Button is pressed
            press();
        else if (stateChange == rising) // Button is released
            release();
    }
    if (state != prevState) // Button is pressed, released or bounces
    {
        prevBounceTime = millis();
        prevState = state;
    }
}