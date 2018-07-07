#pragma once

#include "../../Settings/SettingsWrapper.h"
#include "DigitalMIDIOut.h"

/**
 * @brief   A class for MIDI_Control_Element%s that send MIDI Control Change
 *          on/off values.
 *
 * @see     NoteOut
 */
class DigitalCCOut : public DigitalMIDIOut {
  public:
    /**
     * @brief   Construct a new DigitalCCOut object.
     *
     * @param   address
     *          The MIDI Controller number. [0, 119]
     * @param   channel
     *          The MIDI channel. [1, 16]
     * @param   offValue
     *          The MIDI Controller value in the off state. [0, 127]
     *          (default = 0)
     * @param   onValue
     *          The MIDI Controller value in the on state. [0, 127]
     *          (default = 127)
     */
    DigitalCCOut(uint8_t address, uint8_t channel, uint8_t offValue = 0,
                 uint8_t onValue = 127);

  protected:
    void sendOn();
    void sendOff();

  private:
    bool isActive() const final override;

    const uint8_t address;
    const uint8_t channel;
    const uint8_t offValue;
    const uint8_t onValue;

    bool active = false;
};

// -------------------------------------------------------------------------- //

/**
 * @brief   A class for MIDI_Control_Element%s that send MIDI Control Change
 *          continuous values.
 */
class CCOut : public MIDI_Control_Element {
  public:
    /**
     * @brief   Construct a new CCOut object.
     *
     * @param   address
     *          The MIDI Controller number. [0, 119]
     * @param   channel
     *          The MIDI channel. [1, 16]
     */
    CCOut(uint8_t address, uint8_t channel);

  protected:
    void send(uint8_t value);
    virtual void refresh() = 0;
    void update() override;

  private:
    const uint8_t address;
    const uint8_t channel;
};