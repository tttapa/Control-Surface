#ifndef Control_Surface_CLASS_H
#define Control_Surface_CLASS_H

#include "../MIDI_Interfaces/USBMIDI_Interface.h"
// #include "../Settings/Settings.h"

class Control_Surface_
{
public:
  static Control_Surface_ &getInstance();
  Control_Surface_(Control_Surface_ const &) = delete;
  void operator=(Control_Surface_ const &) = delete;
  ~Control_Surface_();

  void begin();
  void refresh();
  MIDI_Interface *MIDI();

private:
  Control_Surface_() {}
  MIDI_Interface *new_midi = nullptr;
  MIDI_Interface *midi = nullptr;
  void refreshControls();
  void refreshBankSelectors();
#ifndef NO_MIDI_INPUT
  void updateMidiInput();
  void refreshInputs();
#endif
};

extern Control_Surface_ &Control_Surface;

#endif // Control_Surface_CLASS_H