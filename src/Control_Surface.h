/**
 * @brief   The main header file that includes all Control-Surface header files.
 *
 * @file    Control_Surface.h
 * @author  Pieter Pas
 * @date    2018-07-04
 */

#pragma once

#include <Arduino.h> // For VSCode errors in examples

#if defined(CORE_TEENSY) && \
    !(defined(USB_MIDI_AUDIO_SERIAL) || defined(USB_MIDI_SERIAL))
#error "Please select a 'Serial + MIDI' option in the 'Tools > USB Type' menu."
#endif

// -------------------------------------------------------------------------- //
#include <Control_Surface/Control_Surface_Class.h>

// -------------------------------------------------------------------------- //
#include <MIDI_Outputs/CCButton.hpp>
#include <MIDI_Outputs/CCButtonLatching.hpp>
#include <MIDI_Outputs/CCButtonMatrix.hpp>
#include <MIDI_Outputs/CCButtons.hpp>
#include <MIDI_Outputs/CCPotentiometer.hpp>
#include <MIDI_Outputs/NoteButton.hpp>
#include <MIDI_Outputs/NoteButtonLatching.hpp>
#include <MIDI_Outputs/NoteButtonMatrix.hpp>
#include <MIDI_Outputs/NoteButtons.hpp>
#include <MIDI_Outputs/PBPotentiometer.hpp>

#include <MIDI_Outputs/Bankable/CCButton.hpp>
#include <MIDI_Outputs/Bankable/CCButtonLatching.hpp>
#include <MIDI_Outputs/Bankable/CCButtonMatrix.hpp>
#include <MIDI_Outputs/Bankable/CCButtons.hpp>
#include <MIDI_Outputs/Bankable/CCPotentiometer.hpp>
#include <MIDI_Outputs/Bankable/NoteButton.hpp>
#include <MIDI_Outputs/Bankable/NoteButtonLatching.hpp>
#include <MIDI_Outputs/Bankable/NoteButtonMatrix.hpp>
#include <MIDI_Outputs/Bankable/NoteButtons.hpp>
#include <MIDI_Outputs/Bankable/PBPotentiometer.hpp>

#ifdef Encoder_h_
#include <MIDI_Outputs/CCRotaryEncoder.hpp>
#include <MIDI_Outputs/Bankable/CCRotaryEncoder.hpp>
#endif

// -------------------------------------------------------------------------- //
#include <MIDI_Inputs/MCU/SevenSegmentDisplay.hpp>
#include <MIDI_Inputs/MCU/AssignmentDisplay.hpp>
#include <MIDI_Inputs/MCU/VPotRing.hpp>
#include <MIDI_Inputs/MIDINote.hpp>
// #include "MIDI_Inputs/MIDI_Input_VU.h" // TODO

// -------------------------------------------------------------------------- //
#include <MIDI_Inputs/LEDs/MCU/VPotRingLEDs.hpp>
#include <MIDI_Inputs/LEDs/MIDINoteLED.hpp>

// -------------------------------------------------------------------------- //
#include <Banks/Bank.h>
#include <Banks/BankSelector.hpp>

// -------------------------------------------------------------------------- //
#include <Selectors/ProgramSelector.h>
#include <Selectors/Transposer.h>

// -------------------------------------------------------------------------- //
#include <MIDI_Interfaces/USBMIDI_Interface.h>
#include <MIDI_Interfaces/SerialMIDI_Interface.h>
#include <MIDI_Interfaces/DebugMIDI_Interface.h>

// -------------------------------------------------------------------------- //
#include <Hardware/ExtendedInputOutput/ExtendedInputOutput.h>
#include <Hardware/ExtendedInputOutput/ShiftRegisterOut.h>
#include <Hardware/ExtendedInputOutput/AnalogMultiplex.h>

// -------------------------------------------------------------------------- //
#include <MIDI_Constants/MCU.h>
#include <MIDI_Constants/Control_Change.h>

// -------------------------------------------------------------------------- //
#ifdef DISPLAY_GFX
#include "Display/NoteDisplay.hpp"
#include "Display/SelectorDisplay.hpp"
#include "Display/TimeDisplay.hpp"
#include "Display/VPotDisplay.hpp"
#include "Display/VUDisplay.hpp"
#include "Display/Bitmaps/XBitmaps.h"
#endif
