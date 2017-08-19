#include "BankSelector.h"

using namespace ExtIO;

void BankSelector::init()
{
    switch (mode)
    {
    case SINGLE_SWITCH:
    case SINGLE_BUTTON:
        pinMode(switchPin, INPUT_PULLUP);
        break;
    case SINGLE_SWITCH_LED:
    case SINGLE_BUTTON_LED:
        pinMode(switchPin, INPUT_PULLUP);
        pinMode(ledPin, OUTPUT);
        break;
    case MULTIPLE_BUTTONS:
        for (uint8_t i = 0; i < nb_banks; i++)
        {
            pinMode(switchPins[i], INPUT_PULLUP);
        }
        break;
    case MULTIPLE_BUTTONS_LEDS:
        for (uint8_t i = 0; i < nb_banks; i++)
        {
            pinMode(switchPins[i], INPUT_PULLUP);
            pinMode(ledPins[i], OUTPUT);
        }
        digitalWrite(ledPins[0], HIGH);
        break;
    case INCREMENT_DECREMENT:
        pinMode(switchPins[0], INPUT_PULLUP);
        pinMode(switchPins[1], INPUT_PULLUP);
        break;
    case INCREMENT_DECREMENT_LEDS:
        ExtIO::pinMode(switchPins[0], INPUT_PULLUP);
        ExtIO::pinMode(switchPins[1], INPUT_PULLUP);
        for (uint8_t i = 0; i < nb_banks; i++)
        {
            ExtIO::pinMode(ledPins[i], OUTPUT);
        }
        ExtIO::digitalWrite(ledPins[0], HIGH);
        break;
    case INCREMENT:
        pinMode(switchPins[0], INPUT_PULLUP);
        break;
    case INCREMENT_LEDS:
        pinMode(switchPins[0], INPUT_PULLUP);
        for (uint8_t i = 0; i < nb_banks; i++)
        {
            pinMode(ledPins[i], OUTPUT);
        }
        digitalWrite(ledPins[0], HIGH);
        break;
    }
}

void BankSelector::refresh()
{
    uint8_t newChannel = channel;
    switch (mode)
    {
    case SINGLE_SWITCH:
    case SINGLE_SWITCH_LED:
    {
        newChannel = (uint8_t)(!digitalRead(switchPin)) + 1;
    }
    break;

    case SINGLE_BUTTON:
    case SINGLE_BUTTON_LED:
    {
        bool ToggleState = digitalRead(switchPin);

        int8_t stateChangeToggle = ToggleState - prevToggleState;

        if (stateChangeToggle == falling)
        { // Button is pushed
            if (millis() - prevBounceTime > debounceTime)
            {
                newChannel = !(channel - 1) + 1; // Toggle channel between 1 and 2
                prevBounceTime = millis();
            }
        }
        if (stateChangeToggle == rising)
        { // Button is released
            prevBounceTime = millis();
        }
        prevToggleState = ToggleState;
    }
    break;

    case MULTIPLE_BUTTONS:
    case MULTIPLE_BUTTONS_LEDS:
    {
        for (uint8_t i = 0; i < nb_banks; i++)
        {
            if (digitalRead(switchPins[i]) == LOW)
            {
                newChannel = i + 1;
                break;
            }
        }
    }
    break;

    case INCREMENT_DECREMENT_LEDS:
    case INCREMENT_DECREMENT:
    {
        bool IncrementState = digitalRead(switchPins[0]);
        bool DecrementState = digitalRead(switchPins[1]);

        int8_t stateChangeIncrement = IncrementState - prevIncrementState;
        int8_t stateChangeDecrement = DecrementState - prevDecrementState;

        if (stateChangeIncrement == falling)
        { // Increment button is pushed
            if (millis() - prevBounceTime > debounceTime)
            {
                newChannel = channel == nb_banks ? 1 : channel + 1; // Increment channel number or wrap around
                prevBounceTime = millis();
            }
        }
        if (stateChangeDecrement == falling)
        { // Decrement button is pushed
            if (millis() - prevBounceTime > debounceTime)
            {
                newChannel = channel == 1 ? nb_banks : channel - 1; // Decrement channel number or wrap around
                prevBounceTime = millis();
            }
        }
        if (stateChangeIncrement == rising || stateChangeDecrement == rising)
        { // One of the buttons is released
            prevBounceTime = millis();
        }
        prevIncrementState = IncrementState;
        prevDecrementState = DecrementState;
    }
    break;

    case INCREMENT_LEDS:
    case INCREMENT:
    {
        bool IncrementState = digitalRead(switchPins[0]);

        int8_t stateChangeIncrement = IncrementState - prevIncrementState;

        if (stateChangeIncrement == falling)
        { // Increment button is pushed
            if (millis() - prevBounceTime > debounceTime)
            {
                newChannel = channel == nb_banks ? 1 : channel + 1; // Increment channel number or wrap around
                prevBounceTime = millis();
            }
        }
        if (stateChangeIncrement == rising)
        { // Increment button is released
            prevBounceTime = millis();
        }
        prevIncrementState = IncrementState;
    }
    break;
    }
    if (newChannel != channel)
    {
        if (channelChangeEvent != nullptr)
            channelChangeEvent(newChannel);
        refreshLEDs(newChannel);
        channel = newChannel;
        bank.setChannel(channel);
    }
    bank.refresh();
}

uint8_t BankSelector::getChannel()
{
    return channel;
}
void BankSelector::setChannel(uint8_t newChannel)
{
    refreshLEDs(newChannel);
    this->channel = newChannel;
}

const char *BankSelector::getMode()
{
    if (mode == SINGLE_BUTTON)
        return "SINGLE_BUTTON";
    if (mode == SINGLE_BUTTON_LED)
        return "SINGLE_BUTTON_LED";
    if (mode == SINGLE_SWITCH)
        return "SINGLE_SWITCH";
    if (mode == SINGLE_SWITCH_LED)
        return "SINGLE_SWITCH_LED";
    if (mode == MULTIPLE_BUTTONS)
        return "MULTIPLE_BUTTONS";
    if (mode == MULTIPLE_BUTTONS_LEDS)
        return "MULTIPLE_BUTTONS_LEDS";
    if (mode == INCREMENT_DECREMENT)
        return "INCREMENT_DECREMENT";
    if (mode == INCREMENT_DECREMENT_LEDS)
        return "INCREMENT_DECREMENT_LEDS";
    if (mode == INCREMENT)
        return "INCREMENT";
    if (mode == INCREMENT_LEDS)
        return "INCREMENT_LEDS";
    return "";
}

void BankSelector::setChannelChangeEvent(void (*fn)(uint8_t))
{
    channelChangeEvent = fn;
}

void BankSelector::refreshLEDs(uint8_t newChannel)
{
    switch (mode)
    {
    case SINGLE_SWITCH_LED:
    case SINGLE_BUTTON_LED:
    {
        ExtIO::digitalWrite(ledPin, newChannel - 1);
    }
    break;

    case MULTIPLE_BUTTONS_LEDS:
    case INCREMENT_DECREMENT_LEDS:
    case INCREMENT_LEDS:
    {
        ExtIO::digitalWrite(ledPins[channel - 1], LOW);
        ExtIO::digitalWrite(ledPins[newChannel - 1], HIGH);
    }
    break;
    }
}