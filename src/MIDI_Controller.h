#ifndef MIDIctrl_h_
#define MIDIctrl_h_

#include "Arduino.h"

#if defined(CORE_TEENSY) && !(defined(USB_MIDI_AUDIO_SERIAL) || defined(USB_MIDI_SERIAL))
#error Please select a 'Serial + MIDI' option in the 'Tools > USB Type' menu.
#endif

#include "./MIDI_Controller/MIDI_Controller_Class.h"

#include "./Settings/Settings.h"

#include "./MIDI_Outputs/Analog.h"
#include "./MIDI_Outputs/AnalogHiRes.h"
#include "./MIDI_Outputs/ButtonMatrix.h"
#include "./MIDI_Outputs/Digital.h"
#include "./MIDI_Outputs/DigitalLatch.h"
#include "./MIDI_Outputs/RotaryEncoder.h"

#include "./Banks/Bank.h"
#include "./Banks/BankSelector.h"

#include "./MIDI_Interfaces/MIDI_Interface.h"
#include "./MIDI_Interfaces/USBMIDI_Interface.h"
#include "./MIDI_Interfaces/SerialMIDI_Interface.h"
#include "./MIDI_Interfaces/DebugMIDI_Interface.h"

#include "./ExtendedInputOutput/ExtendedInputOutput.h"
#include "./ExtendedInputOutput/ShiftRegisterOut.h"
#include "./ExtendedInputOutput/AnalogMultiplex.h"

#include "./MIDI_Constants/MCU.h"
#include "./MIDI_Constants/Control_Change.h"

#endif // MIDIctrl_h_
