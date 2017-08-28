#ifndef MCU_VPOT_RING_H_
#define MCU_VPOT_RING_H

#include "MIDI_Input_Element.h"
#include "ExtendedInputOutput/ExtendedInputOutput.h"

using namespace ExtIO;

class MCU_VPot_Ring : public MIDI_Input_Element_CC
{
  public:
    MCU_VPot_Ring(uint8_t address, uint8_t nb_addresses)
        : MIDI_Input_Element_CC(address + 0x30, 1, nb_addresses, 1),
          address(address), nb_addresses(nb_addresses)
    {
        initBuffer();
    }

    ~MCU_VPot_Ring()
    {
        free(values);
    }

    bool updateImpl(uint8_t targetAddress, uint8_t targetChannel)
    {
        uint8_t data2 = MIDI_Interface::getDefault()->read();
        uint8_t index = targetAddress - 0x30;
        index = index < nb_addresses ? index : nb_addresses - 1;
        setValue(index, data2);

        display();
        return true;
    }

    uint8_t getPosition()
    {
        return getPosition(addressOffset);
    }
    uint8_t getCenterLED()
    {
        return getCenterLED(addressOffset);
    }
    uint8_t getMode()
    {
        return getMode(addressOffset);
    }

  protected:
    const uint8_t address;
    const uint8_t nb_addresses;
    uint8_t *values = nullptr;

    void initBuffer()
    {
        if (values != nullptr)
            return;
        values = (uint8_t *)malloc(sizeof(uint8_t) * nb_addresses);
        memset(values, 0, sizeof(uint8_t) * nb_addresses);
    }

    void setValue(uint8_t address, uint8_t value)
    {
        values[address] = value;
    }
    uint8_t getPosition(uint8_t address)
    {
        return values[address] & 0x0F;
    }
    bool getCenterLED(uint8_t address)
    {
        return values[address] & 0b01000000;
    }
    uint8_t getMode(uint8_t address)
    {
        return (values[address] & 0b00110000) >> 4;
    }
};

//----------------------------------------------------------------------------------------------------------------------------------------//

class MCU_VPot_Ring_LED : public MCU_VPot_Ring
{
  public:
    MCU_VPot_Ring_LED(pin_t start, uint8_t length, uint8_t address, uint8_t nb_addresses)
        : MCU_VPot_Ring(address, nb_addresses), start(start), centerLEDpin(0), length(length < 12 ? length : 12),
          centerLED(false)
    {
        for (pin_t pin = 0; pin < length; pin++)
            pinMode(start + pin, OUTPUT);
    }
    MCU_VPot_Ring_LED(pin_t start, uint8_t length, pin_t centerLEDpin, uint8_t address, uint8_t nb_addresses)
        : MCU_VPot_Ring(address, nb_addresses), start(start), length(length < 12 ? length : 12),
          centerLEDpin(centerLEDpin), centerLED(true)
    {
        for (pin_t pin = 0; pin < length; pin++)
            pinMode(start + pin, OUTPUT);
        pinMode(centerLEDpin, OUTPUT);
    }
    ~MCU_VPot_Ring_LED()
    {
        for (pin_t pin = 0; pin < length; pin++)
        {
            pinMode(start + pin, INPUT);
        }
        if (centerLED)
            pinMode(centerLEDpin, INPUT);
    }

  protected:
    const uint8_t length;
    const pin_t start, centerLEDpin;
    const bool centerLED;

    void display()
    {
        uint8_t value = getPosition(addressOffset) * length / 12 + 1;

        if (centerLED)
            digitalWrite(centerLEDpin, getCenterLED(addressOffset));
        if (value == 0)
        {
            for (uint8_t pin = 0; pin < length; pin++)
                digitalWrite(start + pin, LOW);
            return;
        }
        switch (getMode(addressOffset))
        {
        case 0:
        {
            for (uint8_t pin = 0; pin < length; pin++)
                digitalWrite(start + pin, LOW);
            digitalWrite(start + value - 1, HIGH);
        }
        break;
        case 1:
        {
            uint8_t startOn = min(value, length / 2) - 1;
            uint8_t startOff = max(value, length / 2);
            for (uint8_t pin = 0; pin < startOn; pin++)
                digitalWrite(start + pin, LOW);
            for (uint8_t pin = startOn; pin < startOff; pin++)
                digitalWrite(start + pin, HIGH);
            for (uint8_t pin = startOff; pin < length; pin++)
                digitalWrite(start + pin, LOW);
        }
        break;
        case 2:
        {
            for (uint8_t pin = 0; pin < value; pin++)
                digitalWrite(start + pin, HIGH);
            for (uint8_t pin = value; pin < length; pin++)
                digitalWrite(start + pin, LOW);
        }
        break;
        case 3:
        {
            uint8_t startOn = max(length / 2 - value, 0) + 1;
            uint8_t startOff = min(length / 2 + value, length);
            for (uint8_t pin = 0; pin < startOn; pin++)
                digitalWrite(start + pin, LOW);
            for (uint8_t pin = startOn; pin < startOff; pin++)
                digitalWrite(start + pin, HIGH);
            for (uint8_t pin = startOff; pin < length; pin++)
                digitalWrite(start + pin, LOW);
        }
        break;
        }
    }

    uint8_t min(uint8_t a, uint8_t b)
    {
        return a > b ? b : a;
    }
    uint8_t max(uint8_t a, uint8_t b)
    {
        return a < b ? b : a;
    }
};
#endif // MCU_VPOT_RING_H_