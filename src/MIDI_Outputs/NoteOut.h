#ifndef CONTROL_SURFACE_MIDI_OUTPUTS_NOTEOUT_H_
#define CONTROL_SURFACE_MIDI_OUTPUTS_NOTEOUT_H_

#include "../Settings/SettingsWrapper.h"
#include "./MIDI_Control_Element.h"
/**
 * @brief
 * A class for MIDI_Control_Element%s that send MIDI notes.
 */
class NoteOut : public MIDI_Control_Element {
  public:
    /**
     * @brief   Construct a new NoteOut object.
     *
     * @param   address
     *          The MIDI Note number (\c messageType == NOTE) [0, 127].
     * @param   channel
     *          The MIDI channel. [1, 16]
     * @param   velocity
     *          The MIDI Note velocity (\c messageType == NOTE) [1, 127].
     */
    NoteOut(uint8_t address, uint8_t channel,
            uint8_t velocity = 127); // Constructor

    void setVelocity(uint8_t velocity);

  protected:
    void sendOn();
    void sendOff();
    virtual void refresh() = 0;

  private:
    /**
     * @brief   Check if the button state changed, and send a
     *          MIDI event accordingly
     */
    void update() override;

    const uint8_t address;
    const uint8_t channel;
    uint8_t velocity;

    void setChannelOffset(uint8_t offset) override;
    void setAddressOffset(uint8_t offset) override;

    bool active = false;
    uint8_t newAddressOffset = addressOffset;
    uint8_t newChannelOffset = channelOffset;
};

#endif // CONTROL_SURFACE_MIDI_OUTPUTS_NOTEOUT_H_
