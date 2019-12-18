/**
 * @file
 * @brief   The main header file that includes all Control-Surface header files.
 *
 * @author  Pieter Pas
 * @date    2018-07-04
 */

#pragma once

#include <Arduino.h> // For VSCode errors in examples

#include <AH/Teensy/TeensyUSBTypes.hpp>

// ------------------------------- Main Logic ------------------------------- //
#include <Control_Surface/Control_Surface_Class.hpp>

// -------------------------------- Display --------------------------------- //
#include <AH/Hardware/LEDs/MAX7219SevenSegmentDisplay.hpp>
#include <Display/Bitmaps/XBitmaps.hpp>
#include <Display/MCU/LCDDisplay.hpp>
#include <Display/MCU/TimeDisplayDisplay.hpp>
#include <Display/MCU/VPotDisplay.hpp>
#include <Display/MCU/VUDisplay.hpp>
#include <Display/NoteBitmapDisplay.hpp>
#include <Display/SelectorDisplay.hpp>

// ------------------------------ MIDI Outputs ------------------------------ //
#include <MIDI_Outputs/CCButton.hpp>
#include <MIDI_Outputs/CCButtonLatched.hpp>
#include <MIDI_Outputs/CCButtonLatching.hpp>
#include <MIDI_Outputs/CCButtonMatrix.hpp>
#include <MIDI_Outputs/CCButtons.hpp>
#include <MIDI_Outputs/CCIncrementDecrementButtons.hpp>

#include <MIDI_Outputs/CCPotentiometer.hpp>

#include <MIDI_Outputs/NoteButton.hpp>
#include <MIDI_Outputs/NoteButtonLatched.hpp>
#include <MIDI_Outputs/NoteButtonLatching.hpp>
#include <MIDI_Outputs/NoteButtonMatrix.hpp>
#include <MIDI_Outputs/NoteButtons.hpp>
#include <MIDI_Outputs/NoteChordButton.hpp>

#include <MIDI_Outputs/PBPotentiometer.hpp>
#include <MIDI_Outputs/PCButton.hpp>

#include <MIDI_Outputs/Bankable/CCButton.hpp>
#include <MIDI_Outputs/Bankable/CCButtonLatched.hpp>
#include <MIDI_Outputs/Bankable/CCButtonLatching.hpp>
#include <MIDI_Outputs/Bankable/CCButtonMatrix.hpp>
#include <MIDI_Outputs/Bankable/CCButtons.hpp>
#include <MIDI_Outputs/ManyAddresses/CCButton.hpp>

#include <MIDI_Outputs/Bankable/CCIncrementDecrementButtons.hpp>
#include <MIDI_Outputs/Bankable/CCPotentiometer.hpp>
#include <MIDI_Outputs/ManyAddresses/CCButtonMatrix.hpp>
#include <MIDI_Outputs/ManyAddresses/CCIncrementDecrementButtons.hpp>
#include <MIDI_Outputs/ManyAddresses/CCPotentiometer.hpp>

#include <MIDI_Outputs/Bankable/NoteButton.hpp>
#include <MIDI_Outputs/Bankable/NoteButtonLatched.hpp>
#include <MIDI_Outputs/Bankable/NoteButtonLatching.hpp>
#include <MIDI_Outputs/Bankable/NoteButtonMatrix.hpp>
#include <MIDI_Outputs/Bankable/NoteButtons.hpp>
#include <MIDI_Outputs/Bankable/NoteChordButton.hpp>
#include <MIDI_Outputs/ManyAddresses/NoteButton.hpp>
#include <MIDI_Outputs/ManyAddresses/NoteButtonMatrix.hpp>

#include <MIDI_Outputs/Bankable/PBPotentiometer.hpp>
#include <MIDI_Outputs/Bankable/PCButton.hpp>
#include <MIDI_Outputs/ManyAddresses/PBPotentiometer.hpp>
#include <MIDI_Outputs/ManyAddresses/PCButton.hpp>

#ifdef Encoder_h_
#include <MIDI_Outputs/Bankable/CCRotaryEncoder.hpp>
#include <MIDI_Outputs/CCAbsoluteEncoder.hpp>
#include <MIDI_Outputs/CCRotaryEncoder.hpp>
#include <MIDI_Outputs/PBAbsoluteEncoder.hpp>
#endif

// ------------------------------ MIDI Inputs ------------------------------- //
// #include <MIDI_Inputs/MCU/AssignmentDisplay.hpp>
// #include <MIDI_Inputs/MCU/SevenSegmentDisplay.hpp>
#include <MIDI_Inputs/MCU/LCD.hpp>
#include <MIDI_Inputs/MCU/VPotRing.hpp>
#include <MIDI_Inputs/MCU/VU.hpp>
#include <MIDI_Inputs/NoteCCRange.hpp>

#include <MIDI_Inputs/LEDs/MCU/VPotRingLEDs.hpp>
#include <MIDI_Inputs/LEDs/MCU/VULEDs.hpp>
#include <MIDI_Inputs/LEDs/NoteCCRangeLEDBar.hpp>
#include <MIDI_Inputs/LEDs/NoteCCRangeLEDs.hpp>

#ifdef FASTLED_VERSION
#include <MIDI_Inputs/LEDs/FastLED.hpp>
#endif

// ------------------------------- Selectors -------------------------------- //
#include <Selectors/IncrementDecrementSelector.hpp>
#include <Selectors/IncrementSelector.hpp>
#include <Selectors/ManyButtonsSelector.hpp>
#include <Selectors/ProgramChangeSelector.hpp>
#include <Selectors/SwitchSelector.hpp>

#ifdef Encoder_h_
#include <Selectors/EncoderSelector.hpp>
#endif

#include <Selectors/LEDs/SelectorLEDs.hpp>

#include <Selectors/ProgramChanger.hpp>
#include <Selectors/Transposer.hpp>

// ---------------------------- MIDI Interfaces ----------------------------- //
#include <MIDI_Interfaces/DebugMIDI_Interface.hpp>
#include <MIDI_Interfaces/MultiMIDI_Interface.hpp>
#include <MIDI_Interfaces/SerialMIDI_Interface.hpp>
#include <MIDI_Interfaces/USBMIDI_Interface.hpp>

// ------------------------- Extended Input Output -------------------------- //
#include <AH/Hardware/ExtendedInputOutput/AnalogMultiplex.hpp>
#include <AH/Hardware/ExtendedInputOutput/ExtendedInputOutput.hpp>
#include <AH/Hardware/ExtendedInputOutput/MAX7219.hpp>
#include <AH/Hardware/ExtendedInputOutput/SPIShiftRegisterOut.hpp>
#include <AH/Hardware/ExtendedInputOutput/ShiftRegisterOut.hpp>

// ----------------------------- MIDI Constants ----------------------------- //
#include <MIDI_Constants/Chords/Chords.hpp>
#include <MIDI_Constants/Control_Change.hpp>
#include <MIDI_Constants/MCU.hpp>
#include <MIDI_Constants/Notes.hpp>
#include <MIDI_Constants/Program_Change.hpp>

// ----------------------------- Array Helpers ------------------------------ //
#include <AH/Containers/ArrayHelpers.hpp>

// --------------------------------- Audio ---------------------------------- //
#ifdef TEENSY_AUDIOUSB_ENABLED
#include <Audio/AudioVU.hpp>
#include <Audio/AudioVULEDs.hpp>
#include <Audio/VolumeControl.hpp>
#endif

BEGIN_CS_NAMESPACE
#ifndef NO_USING_NAMESPACE_AH
USING_AH_NAMESPACE;
#endif
END_CS_NAMESPACE

#ifndef NO_USING_NAMESPACE_CS
USING_CS_NAMESPACE;
#endif

#define CONTROL_SURFACE_VERSION 10100