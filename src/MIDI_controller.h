#ifndef MIDIctrl_h_
#define MIDIctrl_h_

#include "Analog.h"
#include "AnalogHiRes.h"
#include "ControlChange.h"
#include "Digital.h"
#include "DigitalLatch.h"
#include "RotaryEncoder.h"

#include "Bank.h"
#include "BankSelector.h"

#include "MIDI_Interface.h"
#include "USBMIDI_Interface.h"
#include "SerialMIDI_Interface.h"
#include "DebugMIDI_Interface.h"

#include "MIDI_Input_LED.h"

#include "ExtendedInputOutput/ShiftRegisterOut.h"
#include "ExtendedInputOutput/AnalogMultiplex.h"

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
  void updateMidiInput();
  void refreshInputs();
  void refreshBankSelectors();
};

extern MIDI_Controller_ &MIDI_Controller;

#endif // MIDIctrl_h_
