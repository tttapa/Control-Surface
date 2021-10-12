#pragma once

#include <Control_Surface/Control_Surface_Class.hpp>

BEGIN_CS_NAMESPACE

/**
 * @brief   Class that sends MIDI control change messages signifying either
 *          "on" or "off".
 * 
 * @ingroup MIDI_Senders
 */
class DigitalCCSender {
  public:
    DigitalCCSender(uint8_t onValue = 0x7F, uint8_t offValue = 0x00)
        : onValue(onValue), offValue(offValue) {}

    /// Send a control change message to the given address, with @p onValue as
    /// value.
    void sendOn(MIDIAddress address) {
        Control_Surface.sendControlChange(address, onValue);
    }
    /// Send a control change message to the given address, with @p offValue as
    /// value.
    void sendOff(MIDIAddress address) {
        Control_Surface.sendControlChange(address, offValue);
    }

    uint8_t getOnValue() const { return this->onValue; }
    void setOnValue(uint8_t onValue) { this->onValue = onValue; }
    uint8_t getOffValue() const { return this->offValue; }
    void setOffValue(uint8_t offValue) { this->offValue = offValue; }

  private:
    uint8_t onValue;
    uint8_t offValue;
};

END_CS_NAMESPACE