#ifndef CONTROL_SURFACE_MIDI_OUTPUTS_PBOUT_H_
#define CONTROL_SURFACE_MIDI_OUTPUTS_PBOUT_H_

#include "../Settings/SettingsWrapper.h"
#include "./MIDI_Control_Element.h"

/**
 * @brief
 * A class for MIDI_Control_Element%s that send MIDI Control Change
 * continuous values.
 */
class PBOut : public MIDI_Control_Element {
  public:
    /**
     * @brief   Construct a new PBOut object.
     *
     * @param   channel
     *          The MIDI channel. [1, 16]
     */
    PBOut(uint8_t channel); // Constructor

  protected:
    void send(uint16_t value);
    virtual void refresh() = 0;
    void update() override; // Read the new continuous value

  private:
    const uint8_t channel;
};

#endif // CONTROL_SURFACE_MIDI_OUTPUTS_PBOUT_H_
