#pragma once

#include "Abstract_MIDI_Output.hpp"
#include <Control_Surface/Control_Surface_Class.h>

class Abstract_MIDI_Output_Addressable : public Abstract_MIDI_Output {
  public:
    virtual uint8_t getAddress(uint8_t baseAddress) const {
        return baseAddress;
    }
};

// -------------------------------------------------------------------------- //

class Abstract_Digital_MIDI_Output_Addressable
    : public virtual Abstract_MIDI_Output_Addressable {
  public:
    virtual void sendOn(uint8_t channel, uint8_t address) const = 0;
    virtual void sendOff(uint8_t channel, uint8_t address) const = 0;
};

// -------------------------------------------------------------------------- //

class Abstract_Digital_CC_Output
    : public virtual Abstract_Digital_MIDI_Output_Addressable {
  public:
    void sendOn(uint8_t channel, uint8_t address) const override {
        Control_Surface.MIDI().send(CC, channel, address, onValue);
    }
    void sendOff(uint8_t channel, uint8_t address) const override {
        Control_Surface.MIDI().send(CC, channel, address, offValue);
    }

  private:
    const static uint8_t onValue = 127;
    const static uint8_t offValue = 0;
};

// -------------------------------------------------------------------------- //

class Abstract_Digital_Note_Output
    : public virtual Abstract_Digital_MIDI_Output_Addressable {
  public:
    void sendOn(uint8_t channel, uint8_t address) const override {
        Control_Surface.MIDI().send(NOTE_ON, channel, address, velocity);
    }
    void sendOff(uint8_t channel, uint8_t address) const override {
        Control_Surface.MIDI().send(NOTE_OFF, channel, address, velocity);
    }
    void setVelocity(uint8_t velocity) { this->velocity = velocity; }

  private:
    uint8_t velocity = 127;
};

// -------------------------------------------------------------------------- //

