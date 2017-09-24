#ifndef MIDI_CONTROLLER_CLASS_H
#define MIDI_CONTROLLER_CLASS_H

#include "../MIDI_Interfaces/USBMIDI_Interface.h"

class MIDI_Controller_
{
public:
  static MIDI_Controller_ &getInstance();
  MIDI_Controller_(MIDI_Controller_ const &) = delete;
  void operator=(MIDI_Controller_ const &) = delete;
  ~MIDI_Controller_();

  void begin();
  void refresh();
  MIDI_Interface *MIDI();

private:
  MIDI_Controller_() {}
  MIDI_Interface *new_midi = nullptr;
  MIDI_Interface *midi = nullptr;
  void refreshControls();
  void refreshBankSelectors();
};

extern MIDI_Controller_ &MIDI_Controller;

#endif // MIDI_CONTROLLER_CLASS_H