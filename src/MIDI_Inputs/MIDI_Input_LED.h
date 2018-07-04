#pragma once

#include "../Hardware/ExtendedInputOutput/ExtendedInputOutput.h"
#include "../MIDI_Inputs/MIDI_Input_Element.h"
#include "../MIDI_Interfaces/MIDI_Interface.h"

using namespace ExtIO;

class MIDI_Input_Note_Buffer : public MIDI_Input_Element_Note {
  public:
    MIDI_Input_Note_Buffer(uint8_t address, uint8_t channel,
                           size_t nb_addresses = 1, size_t nb_channels = 1);
    ~MIDI_Input_Note_Buffer();
    bool updateImpl(const MIDI_message_matcher &midimsg);
    bool getState();

  protected:
    uint8_t *states = nullptr;
};

class MIDI_LED : public MIDI_Input_Note_Buffer {
  public:
    MIDI_LED(pin_t pin, uint8_t address, uint8_t channel,
             size_t nb_addresses = 1, size_t nb_channels = 1);
    ~MIDI_LED();
    void display();

  protected:
    void update() override;
    pin_t pin;
};