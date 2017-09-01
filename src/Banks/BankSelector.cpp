#include "BankSelector.h"

using namespace ExtIO;

BankSelector *BankSelector::last = nullptr;
BankSelector *BankSelector::first = nullptr;

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
        for (uint8_t i = 0; i < nb_settings; i++)
        {
            pinMode(switchPins[i], INPUT_PULLUP);
        }
        break;
    case MULTIPLE_BUTTONS_LEDS:
        for (uint8_t i = 0; i < nb_settings; i++)
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
        pinMode(switchPins[0], INPUT_PULLUP);
        pinMode(switchPins[1], INPUT_PULLUP);
        for (uint8_t i = 0; i < nb_settings; i++)
        {
            pinMode(ledPins[i], OUTPUT);
        }
        digitalWrite(ledPins[0], HIGH);
        break;
    case INCREMENT:
        pinMode(switchPins[0], INPUT_PULLUP);
        break;
    case INCREMENT_LEDS:
        pinMode(switchPins[0], INPUT_PULLUP);
        for (uint8_t i = 0; i < nb_settings; i++)
        {
            pinMode(ledPins[i], OUTPUT);
        }
        digitalWrite(ledPins[0], HIGH);
        break;
    }
}

void BankSelector::refresh()
{
    uint8_t newBankSetting = bankSetting;
    switch (mode)
    {
    case SINGLE_SWITCH:
    case SINGLE_SWITCH_LED:
    {
        newBankSetting = (uint8_t)(!digitalRead(switchPin));
    }
    break;

    case SINGLE_BUTTON:
    case SINGLE_BUTTON_LED:
    {
        if (debounceButton(dbButton1))
            newBankSetting = !bankSetting; // Toggle bankSetting between 0 and 1
    }
    break;

    case MULTIPLE_BUTTONS:
    case MULTIPLE_BUTTONS_LEDS:
    {
        for (uint8_t i = 0; i < nb_settings; i++)
        {
            if (digitalRead(switchPins[i]) == LOW)
            {
                newBankSetting = i;
                break;
            }
        }
    }
    break;

    case INCREMENT_DECREMENT_LEDS:
    case INCREMENT_DECREMENT:
    {
        if (debounceButton(dbButton1))
            newBankSetting = bankSetting < nb_settings - 1 ? bankSetting + 1 : 0; // Increment bankSetting number or wrap around
        if (debounceButton(dbButton2))
            newBankSetting = bankSetting == 0 ? nb_settings - 1 : bankSetting - 1; // Decrement bankSetting number or wrap around
    }
    break;

    case INCREMENT_LEDS:
    case INCREMENT:
    {
        if (debounceButton(dbButton1))
            newBankSetting = bankSetting < nb_settings - 1 ? bankSetting + 1 : 0; // Increment bankSetting number or wrap around
    }
    break;
    }

    if (newBankSetting != bankSetting)
    {
        if (bankSettingChangeEvent != nullptr)
            bankSettingChangeEvent(newBankSetting);
        refreshLEDs(newBankSetting);
        bankSetting = newBankSetting;
        bank.setBankSetting(bankSetting);
    }
}

uint8_t BankSelector::getBankSetting()
{
    return bankSetting;
}
void BankSelector::setBankSetting(uint8_t newBankSetting)
{
    refreshLEDs(newBankSetting);
    this->bankSetting = newBankSetting;
}

#ifdef DEBUG
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
#endif

void BankSelector::setBankSettingChangeEvent(void (*fn)(uint8_t))
{
    bankSettingChangeEvent = fn;
}

void BankSelector::refreshLEDs(uint8_t newBankSetting)
{
    switch (mode)
    {
    case SINGLE_SWITCH_LED:
    case SINGLE_BUTTON_LED:
    {
        digitalWrite(ledPin, newBankSetting);
    }
    break;

    case MULTIPLE_BUTTONS_LEDS:
    case INCREMENT_DECREMENT_LEDS:
    case INCREMENT_LEDS:
    {
        digitalWrite(ledPins[bankSetting], LOW);
        digitalWrite(ledPins[newBankSetting], HIGH);
    }
    break;
    }
}

bool BankSelector::debounceButton(debouncedButton &button)
{
    bool pressed = false;
    bool state = digitalRead(button.pin);

    int8_t stateChange = state - button.prevState;

    if (stateChange == falling)
    { // Button is pushed
        if (millis() - prevBounceTime > debounceTime)
        {
            pressed = true;
            prevBounceTime = millis();
        }
    }
    if (stateChange == rising)
    { // Button is released
        prevBounceTime = millis();
    }
    button.prevState = state;
    return pressed;
}