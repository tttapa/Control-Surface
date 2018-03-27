#include "./MIDI_Input_V-Pot_Ring.h"
#include "../Helpers/StreamOut.h"
#include "../Helpers/Copy.hpp"
#include <string.h>

using namespace ExtIO;

//  public:

MCU_VPot_Ring::MCU_VPot_Ring(uint8_t track, uint8_t nb_tracks)
    : MIDI_Input_Element_CC(track + 0x30 - 1, 1, nb_tracks, 1),
      track(track), nb_tracks(nb_tracks)
{
    values = new uint8_t[nb_tracks];
    memset(values, 0, sizeof(uint8_t) * nb_tracks);
}

MCU_VPot_Ring::~MCU_VPot_Ring()
{
    delete[] values;
}

bool MCU_VPot_Ring::updateImpl(const MIDI_message_matcher &midimsg)
{
    uint8_t index = (midimsg.data1 - address) / channelsPerBank;
    uint8_t data2 = (midimsg.data2 & 0xF0) | ((midimsg.data2 & 0x0F) < 0x0C ? midimsg.data2 : 0xB);
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

// private:

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

// TODO: replace address with track

// TODO: Should outputs be reset to input mode in destructor?

MCU_VPot_Ring_LED::MCU_VPot_Ring_LED(const pin_t (&LEDs)[11], uint8_t track, uint8_t nb_tracks)
    : MCU_VPot_Ring(track, nb_tracks), centerLEDpin(0), centerLED(false)
{
    copy(this->LEDs, LEDs);
    for (pin_t pin = 0; pin < 11; pin++)
        ExtIO::pinMode(LEDs[pin], OUTPUT);
}
MCU_VPot_Ring_LED::MCU_VPot_Ring_LED(const pin_t (&LEDs)[11], pin_t centerLEDpin, uint8_t track, uint8_t nb_tracks)
    : MCU_VPot_Ring(track, nb_tracks),
      centerLEDpin(centerLEDpin), centerLED(true)
{
    copy(this->LEDs, LEDs);
    for (pin_t pin = 0; pin < 11; pin++)
        ExtIO::pinMode(LEDs[pin], OUTPUT);
    ExtIO::pinMode(centerLEDpin, OUTPUT);
}
MCU_VPot_Ring_LED::~MCU_VPot_Ring_LED()
{
    for (pin_t pin = 0; pin < 11; pin++)
    {
        ExtIO::pinMode(LEDs[pin], INPUT);
    }
    if (centerLED)
        ExtIO::pinMode(centerLEDpin, INPUT);
}

// protected:

void MCU_VPot_Ring_LED::display()
{
    if (centerLED)
        ExtIO::digitalWrite(centerLEDpin, getCenterLED());
    if (getPosition() == 0)
    {
        for (uint8_t pin = 0; pin < 11; pin++)
            ExtIO::digitalWrite(LEDs[pin], LOW);
        return;
    }
    uint8_t value = getPosition() - 1;
#ifdef DEBUG
    DEBUG_OUT << "Display: " << value << endl;
#endif
    switch (getMode())
    {
    case 0:
    {
        for (uint8_t pin = 0; pin < 11; pin++)
            ExtIO::digitalWrite(LEDs[pin], LOW);
        ExtIO::digitalWrite(LEDs[value], HIGH);
    }
    break;
    case 1:
    {
        uint8_t startOn = minimum(value, 5);
        uint8_t startOff = maximum(value, 5) + 1;
        for (uint8_t pin = 0; pin < startOn; pin++)
            ExtIO::digitalWrite(LEDs[pin], LOW);
        for (uint8_t pin = startOn; pin < startOff; pin++)
            ExtIO::digitalWrite(LEDs[pin], HIGH);
        for (uint8_t pin = startOff; pin < 11; pin++)
            ExtIO::digitalWrite(LEDs[pin], LOW);
    }
    break;
    case 2:
    {
        for (uint8_t pin = 0; pin <= value; pin++)
            ExtIO::digitalWrite(LEDs[pin], HIGH);
        for (uint8_t pin = value + 1; pin < 11; pin++)
            ExtIO::digitalWrite(LEDs[pin], LOW);
    }
    break;
    case 3:
    {
        uint8_t startOn = maximum(5 - value, 0);
        uint8_t startOff = minimum(6 + value, 11);
        for (uint8_t pin = 0; pin < startOn; pin++)
            ExtIO::digitalWrite(LEDs[pin], LOW);
        for (uint8_t pin = startOn; pin < startOff; pin++)
            ExtIO::digitalWrite(LEDs[pin], HIGH);
        for (uint8_t pin = startOff; pin < 11; pin++)
            ExtIO::digitalWrite(LEDs[pin], LOW);
    }
    break;
    }
}

// TODO: why can't I use the min/max macros on Teensy?
inline int8_t MCU_VPot_Ring_LED::minimum(int8_t a, int8_t b)
{
    return a > b ? b : a;
}
inline int8_t MCU_VPot_Ring_LED::maximum(int8_t a, int8_t b)
{
    return a < b ? b : a;
}
