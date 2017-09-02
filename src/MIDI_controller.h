#ifndef MIDIctrl_h_
#define MIDIctrl_h_

#include "Arduino.h"

#include "./Settings/Settings.h"

#include "./MIDI_Element.h"

#include "./MIDI_Outputs/Analog.h"
#include "./MIDI_Outputs/AnalogHiRes.h"
#include "./MIDI_Outputs/Digital.h"
#include "./MIDI_Outputs/DigitalLatch.h"
#include "./MIDI_Outputs/RotaryEncoder.h"

#ifndef NO_MIDI_INPUT
#include "./MIDI_Inputs/MIDI_Input_Element.h"
#include "./MIDI_Inputs/MIDI_Input_LED.h"
#include "./MIDI_Inputs/MIDI_Input_V-Pot_Ring.h"
#include "./MIDI_Inputs/MIDI_Input_VU.h"
#include "./MIDI_Inputs/MIDI_Input_7SegmentDisplay.h"
#endif

#include "./Banks/Bank.h"
#include "./Banks/BankSelector.h"

#include "./MIDI_Interfaces/MIDI_Interface.h"
#include "./MIDI_Interfaces/USBMIDI_Interface.h"
#include "./MIDI_Interfaces/SerialMIDI_Interface.h"
#include "./MIDI_Interfaces/DebugMIDI_Interface.h"

#include "./ExtendedInputOutput/ExtendedInputOutput.h"
#include "./ExtendedInputOutput/ShiftRegisterOut.h"
#include "./ExtendedInputOutput/AnalogMultiplex.h"

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
#ifndef NO_MIDI_INPUT
  void updateMidiInput();
  void refreshInputs();
#endif
  void refreshBankSelectors();
};

extern MIDI_Controller_ &MIDI_Controller;

#endif // MIDIctrl_h_
