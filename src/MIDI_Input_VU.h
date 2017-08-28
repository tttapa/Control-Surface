#ifndef MIDI_Input_VU_H_
#define MIDI_Input_VU_H_

#include "MIDI_Input_Element.h"
#include "ExtendedInputOutput/ExtendedInputOutput.h"

using namespace ExtIO;

class MCU_VU : public MIDI_Input_Element_ChannelPressure
{
  public:
    MCU_VU(pin_t start, uint8_t length, uint8_t address, uint8_t nb_addresses, bool decay = true)
        : MIDI_Input_Element_ChannelPressure(1, 1), start(start), overloadpin(0), length(length < 12 ? length : 12),
          address(address), nb_addresses(nb_addresses),
          overload(false), decayTime(1800 / length), decay(decay)
    {
        for (pin_t pin = 0; pin < length; pin++)
        {
            pinMode(start + pin, OUTPUT);
        }
        initBuffer();
    }
    MCU_VU(pin_t start, uint8_t length, pin_t overloadpin, uint8_t address, uint8_t nb_addresses, bool decay = true)
        : MIDI_Input_Element_ChannelPressure(1, 1), start(start), length(length < 12 ? length : 12), 
          overloadpin(overloadpin), address(address), nb_addresses(nb_addresses),
          overload(true), decayTime(1800 / length), decay(decay)
    {
        for (pin_t pin = 0; pin < length; pin++)
        {
            pinMode(start + pin, OUTPUT);
        }
        pinMode(overloadpin, OUTPUT);
        initBuffer();
    }

    ~MCU_VU()
    {
        free(values);
    }
    bool updateImpl(uint8_t targetChannel, uint8_t data1)
    {
        uint8_t targetAddress = data1 >> 4;
        if (!match(targetAddress))
            return false;

        uint8_t data = data1 & 0xF;
        if (data == 0xF) // clear overload
            clearOverload(targetAddress);
        else if (data == 0xE) // set overload
            setOverload(targetAddress);
        else if (data == 0xD) // not implemented
            ; 
        else // new peak value
            setValue(targetAddress, data * length / 12);

        display();
        return true;
    }

    void refresh()
    {
        if (decay && ((millis() - prevDecayTime) > decayTime))
        {
            for (uint8_t i = 0; i < nb_addresses; i++)
                if (getValue(i) > 0)
                    setValue(i, getValue(i) - 1);
            prevDecayTime = millis();
            display();
        }
    }

    uint8_t getValue()
    {
        return getValue(addressOffset);
    }
    uint8_t getOverload()
    {
        return getOverload(addressOffset);
    }

  protected:
    const uint8_t length;
    const uint8_t address;
    const uint8_t nb_addresses;
    uint8_t *values = nullptr;
    const pin_t start, overloadpin;
    const bool decay;
    const bool overload;
    const unsigned long decayTime;
    unsigned long prevDecayTime = 0;

    void initBuffer()
    {
        if (values != nullptr)
            return;
        values = (uint8_t *)malloc(sizeof(uint8_t) * length);
        memset(values, 0, sizeof(uint8_t) * length);
    }

    void setValue(uint8_t address, uint8_t value)
    {
        values[address] &= 0xF0;
        values[address] |= value < length ? value : length;
        prevDecayTime = millis();
    }
    uint8_t getValue(uint8_t address)
    {
        return values[address] & 0x0F;
    }
    void setOverload(uint8_t address)
    {
        values[address] |= 0xF0;
    }
    void clearOverload(uint8_t address)
    {
        values[address] &= 0x0F;
    }
    bool getOverload(uint8_t address)
    {
        return values[address] & 0xF0;
    }
    inline bool match(uint8_t targetAddress)
    {
        return 
           (targetAddress >= this->address) 
        && (targetAddress < this->address + nb_addresses);
    }

    void display()
    {
        for (uint8_t pin = 0; pin < getValue(addressOffset); pin++)
            digitalWrite(start + pin, HIGH);
        for (uint8_t pin = getValue(addressOffset); pin < length; pin++)
            digitalWrite(start + pin, LOW);
        if (overload)
            digitalWrite(overloadpin, getOverload(addressOffset));
    }


};
#endif // MIDI_Input_VU_H_