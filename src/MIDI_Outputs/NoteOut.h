#ifndef CONTROL_SURFACE_MIDI_OUTPUTS_NOTEOUT_H_
#define CONTROL_SURFACE_MIDI_OUTPUTS_NOTEOUT_H_

#include "./MIDI_Control_Element.h"
#include "../Settings/SettingsWrapper.h"
/**
 * @brief A class for MIDI_Control_Element%s that read from a digital input 
 * and send MIDI Note events.
 * 
 * The input is debounced, the debounce time can be changed in Settings.h.
 */
class NoteOut : public MIDI_Control_Element
{
  /**
   * @brief Construct a new NoteOut object.
   * 
   * @param address The MIDI Note number (\c messageType == NOTE) [0, 127].
   * @param channel The MIDI channel. [1, 16]
   * @param velocity The MIDI Note velocity (\c messageType == NOTE) [1, 127].
   */
  NoteOut(uint8_t address, uint8_t channel, uint8_t velocity = 127); // Constructor

  void setVelocity(uint8_t velocity);

protected:
  void sendOn();
  void sendOff();
  virtual void refresh() = 0;

private:
  void update(); // Check if the button state changed, and send a MIDI event accordingly

  const uint8_t address;
  const uint8_t channel;
  uint8_t velocity;

  void setChannelOffset(uint8_t offset);
  void setAddressOffset(uint8_t offset);

  bool active = false;
  uint8_t newAddressOffset = addressOffset;
  uint8_t newChannelOffset = channelOffset;
};

#endif // CONTROL_SURFACE_MIDI_OUTPUTS_NOTEOUT_H_
