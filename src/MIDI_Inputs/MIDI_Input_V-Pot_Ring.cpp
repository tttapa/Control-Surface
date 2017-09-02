#include "../MIDI_controller.h"

#ifndef NO_MIDI_INPUT

using namespace ExtIO;

//  public:

MCU_VPot_Ring::MCU_VPot_Ring(uint8_t address, uint8_t nb_addresses)
    : MIDI_Input_Element_CC(address + 0x30, 1, nb_addresses, 1),
      address(address), nb_addresses(nb_addresses)
{
    initBuffer();
}

MCU_VPot_Ring::~MCU_VPot_Ring()
{
    free(values);
}

bool MCU_VPot_Ring::updateImpl(uint8_t header, uint8_t data1)
{
    uint8_t data2 = MIDI_Controller.MIDI()->read();
    uint8_t index = data1 - 0x30;
    index = index < nb_addresses ? index : nb_addresses - 1;
    setValue(index, data2);

    display();
    return true;
}

uint8_t MCU_VPot_Ring::getPosition()
{
    return getPosition(addressOffset);
}
uint8_t MCU_VPot_Ring::getCenterLED()
{
    return getCenterLED(addressOffset);
}
uint8_t MCU_VPot_Ring::getMode()
{
    return getMode(addressOffset);
}

// protected:

void MCU_VPot_Ring::initBuffer()
{
    if (values != nullptr)
        return;
    values = (uint8_t *)malloc(sizeof(uint8_t) * nb_addresses);
    memset(values, 0, sizeof(uint8_t) * nb_addresses);
}

void MCU_VPot_Ring::setValue(uint8_t address, uint8_t value)
{
    values[address] = value;
}
uint8_t MCU_VPot_Ring::getPosition(uint8_t address)
{
    return values[address] & 0x0F;
}
bool MCU_VPot_Ring::getCenterLED(uint8_t address)
{
    return values[address] & 0b01000000;
}
uint8_t MCU_VPot_Ring::getMode(uint8_t address)
{
    return (values[address] & 0b00110000) >> 4;
}

//----------------------------------------------------------------------------------------------------------------------------------------//

// public:

MCU_VPot_Ring_LED::MCU_VPot_Ring_LED(const pin_t (&LEDs)[11], uint8_t address, uint8_t nb_addresses)
    : MCU_VPot_Ring(address, nb_addresses), LEDs(LEDs), centerLEDpin(0), centerLED(false)
{
    for (pin_t pin = 0; pin < 11; pin++)
        pinMode(LEDs[pin], OUTPUT);
}
MCU_VPot_Ring_LED::MCU_VPot_Ring_LED(const pin_t (&LEDs)[11], pin_t centerLEDpin, uint8_t address, uint8_t nb_addresses)
    : MCU_VPot_Ring(address, nb_addresses), LEDs(LEDs),
      centerLEDpin(centerLEDpin), centerLED(true)
{
    for (pin_t pin = 0; pin < 11; pin++)
        pinMode(LEDs[pin], OUTPUT);
    pinMode(centerLEDpin, OUTPUT);
}
MCU_VPot_Ring_LED::~MCU_VPot_Ring_LED()
{
    for (pin_t pin = 0; pin < 11; pin++)
    {
        pinMode(LEDs[pin], INPUT);
    }
    if (centerLED)
        pinMode(centerLEDpin, INPUT);
}

// protected:

void MCU_VPot_Ring_LED::display()
{
    if (centerLED)
        digitalWrite(centerLEDpin, getCenterLED(addressOffset));
    if (getPosition(addressOffset) == 0)
    {
        for (uint8_t pin = 0; pin < 11; pin++)
            digitalWrite(LEDs[pin], LOW);
        return;
    }
    uint8_t value = getPosition(addressOffset) - 1;
#ifdef DEBUG
    Serial.printf("Display: %d", value);
#endif
    switch (getMode(addressOffset))
    {
    case 0:
    {
        for (uint8_t pin = 0; pin < 11; pin++)
            digitalWrite(LEDs[pin], LOW);
        digitalWrite(LEDs[value], HIGH);
    }
    break;
    case 1:
    {
        uint8_t startOn = minimum(value, 5);
        uint8_t startOff = maximum(value, 5) + 1;
        for (uint8_t pin = 0; pin < startOn; pin++)
            digitalWrite(LEDs[pin], LOW);
        for (uint8_t pin = startOn; pin < startOff; pin++)
            digitalWrite(LEDs[pin], HIGH);
        for (uint8_t pin = startOff; pin < 11; pin++)
            digitalWrite(LEDs[pin], LOW);
    }
    break;
    case 2:
    {
        for (uint8_t pin = 0; pin <= value; pin++)
            digitalWrite(LEDs[pin], HIGH);
        for (uint8_t pin = value + 1; pin < 11; pin++)
            digitalWrite(LEDs[pin], LOW);
    }
    break;
    case 3:
    {
        uint8_t startOn = maximum(5 - value, 0);
        uint8_t startOff = minimum(6 + value, 11);
        for (uint8_t pin = 0; pin < startOn; pin++)
            digitalWrite(LEDs[pin], LOW);
        for (uint8_t pin = startOn; pin < startOff; pin++)
            digitalWrite(LEDs[pin], HIGH);
        for (uint8_t pin = startOff; pin < 11; pin++)
            digitalWrite(LEDs[pin], LOW);
    }
    break;
    }
}

inline int8_t MCU_VPot_Ring_LED::minimum(int8_t a, int8_t b)
{
    return a > b ? b : a;
}
inline int8_t MCU_VPot_Ring_LED::maximum(int8_t a, int8_t b)
{
    return a < b ? b : a;
}

#endif // #ifndef NO_MIDI_INPUT