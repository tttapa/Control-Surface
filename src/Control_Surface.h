#ifndef Control_Surface_H
#define Control_Surface_H

#include <Arduino.h>

#if defined(CORE_TEENSY) && !(defined(USB_MIDI_AUDIO_SERIAL) || defined(USB_MIDI_SERIAL))
#error Please select a 'Serial + MIDI' option in the 'Tools > USB Type' menu.
#endif

#include "./Control_Surface/Control_Surface_Class.h"

#include "./MIDI_Outputs/Analog.h"
#include "./MIDI_Outputs/ButtonMatrix.h"
#include "./MIDI_Outputs/Digital.h"
#include "./MIDI_Outputs/DigitalLatch.h"
#include "./MIDI_Outputs/RotaryEncoder.h"

#include "./MIDI_Inputs/MIDI_Input_Element.h"
#include "./MIDI_Inputs/MIDI_Input_LED.h"
#include "./MIDI_Inputs/MIDI_Input_V-Pot_Ring.h"
#include "./MIDI_Inputs/MIDI_Input_VU.h"
#include "./MIDI_Inputs/MIDI_Input_7SegmentDisplay.h"

#include "./Banks/Bank.h"
#include "./Banks/BankSelector.h"

#include "./Selectors/SelectorPC.h"

#include "./MIDI_Interfaces/USBMIDI_Interface.h"
#include "./MIDI_Interfaces/SerialMIDI_Interface.h"
#include "./MIDI_Interfaces/DebugMIDI_Interface.h"

#include "./ExtendedInputOutput/ExtendedInputOutput.h"
#include "./ExtendedInputOutput/ShiftRegisterOut.h"
#include "./ExtendedInputOutput/AnalogMultiplex.h"

#include "./MIDI_Constants/MCU.h"
#include "./MIDI_Constants/Control_Change.h"

#include "./Helpers/StreamOut.h"

#endif // Control_Surface_H
