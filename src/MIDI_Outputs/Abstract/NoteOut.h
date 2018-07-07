#pragma once

#include "../../Settings/SettingsWrapper.h"
#include "DigitalMIDIOut.h"
/**
 * @brief
 * A class for MIDI_Control_Element%s that send MIDI notes.
 */
class NoteOut : public DigitalMIDIOut {
  public:
    /**
     * @brief   Construct a new NoteOut object.
     *
     * @param   address
     *          The MIDI Note number. [0, 127]
     * @param   channel
     *          The MIDI channel. [1, 16]
     * @param   velocity
     *          The MIDI Note velocity. [1, 127]
     */
    NoteOut(uint8_t address, uint8_t channel,
            uint8_t velocity = 127);

    void setVelocity(uint8_t velocity);

  protected:
    void sendOn();
    void sendOff();

  private:
    bool isActive() const final override;

    const uint8_t address;
    const uint8_t channel;
    uint8_t velocity;

    bool active = false;
};