#ifndef MCU_VPOT_RING_H_
#define MCU_VPOT_RING_H_

#include "Arduino.h"

class MCU_VPot_Ring : public MIDI_Input_Element_CC
{
  public:
    MCU_VPot_Ring(uint8_t address, uint8_t nb_addresses);
    ~MCU_VPot_Ring();

    bool updateImpl(uint8_t targetAddress, uint8_t targetChannel);

    uint8_t getPosition();
    uint8_t getCenterLED();
    uint8_t getMode();

  protected:
    const uint8_t address;
    const uint8_t nb_addresses;
    uint8_t *values = nullptr;

    void initBuffer();

    void setValue(uint8_t address, uint8_t value);
    uint8_t getPosition(uint8_t address);
    bool getCenterLED(uint8_t address);
    uint8_t getMode(uint8_t address);
};

//----------------------------------------------------------------------------------------------------------------------------------------//

class MCU_VPot_Ring_LED : public MCU_VPot_Ring
{
  public:
    MCU_VPot_Ring_LED(const pin_t (&LEDs)[11], uint8_t address, uint8_t nb_addresses);
    MCU_VPot_Ring_LED(const pin_t (&LEDs)[11], pin_t centerLEDpin, uint8_t address, uint8_t nb_addresses);
    ~MCU_VPot_Ring_LED();

  protected:
    const pin_t *LEDs, centerLEDpin;
    const bool centerLED;

    void display();
    inline int8_t minimum(int8_t a, int8_t b);
    inline int8_t maximum(int8_t a, int8_t b);
};

#endif // MCU_VPOT_RING_H_