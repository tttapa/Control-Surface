include(GoogleTest)

# Test executable compilation and linking
add_executable(tests
    "test_example.cpp"
    "test-main.cpp"
    "AH/PrintStream/test-PrintStream.cpp"
    "AH/Timing/test-Timer.cpp"
    "AH/Hardware/test-FilteredAnalog.cpp"
    "AH/Hardware/ExtendedInputOutput/test-AnalogMultiplex.cpp"
    "AH/Hardware/ExtendedInputOutput/test-ExtendedInputOutput.cpp"
    "AH/Hardware/test-IncrementDecrementButtons.cpp"
    "AH/Hardware/test-IncrementButton.cpp"
    "AH/Hardware/test-Button.cpp"
    "AH/Containers/test-Updatable.cpp"
    "AH/Containers/test-DoublyLinkedList.cpp"
    "AH/Containers/test-Array.cpp"
    "AH/Containers/tests-BitArray.cpp"
    "AH/Math/test-Degrees.cpp"
    "AH/Math/test-Quaternion.cpp"
    "AH/Math/test-IncreaseBitDepth.cpp"
    "AH/Math/test-Vector.cpp"
    "AH/Filters/test-Hysteresis.cpp"
    "AH/Filters/test-EMA.cpp"

    "Helpers/test-MIDICNCHannelAddress.cpp"
    "MIDI_Inputs/test-MIDINote.cpp"
    "MIDI_Inputs/test-NoteCCKPLEDBar.cpp"
    "MIDI_Inputs/test-MCU_LCD.cpp"
    "MIDI_Inputs/tests-MCU_VPot.cpp"
    "MIDI_Inputs/tests-MCU_VU.cpp"
    "MIDI_Inputs/test-MCU_TimeDisplay.cpp"
    "MIDI_Inputs/test-MIDIInputElement.cpp"
    "MIDI_Senders/test-RelativeCCSender.cpp"
    "MIDI_Parsers/tests-MIDI_Parsers.cpp"
    "MIDI_Constants/test-MCU.cpp"
    "MIDI_Constants/test-Notes.cpp"
    "MIDI_Outputs/test-PBPotentiometer.cpp"
    "MIDI_Outputs/test-NoteButtonMatrix.cpp"
    "MIDI_Outputs/test-NoteButtonLatching.cpp"
    "MIDI_Outputs/test-CCButton.cpp"
    "MIDI_Outputs/test-CCPotentiometer.cpp"
    "MIDI_Outputs/test-NoteButton.cpp"
    "MIDI_Outputs/test-Construction.cpp"
    "MIDI_Outputs/test-CCRotaryEncoder.cpp"
    "MIDI_Outputs/test-ProgramChanger.cpp"
    "MIDI_Interfaces/test-DebugStreamMIDI_Interface.cpp"
    "MIDI_Interfaces/test-USBMIDI_Interface.cpp"
    "MIDI_Interfaces/test-StreamMIDI_Interface.cpp"
    "MIDI_Interfaces/test-BluetoothMIDI_Interface.cpp"
    "MIDI_Interfaces/test-MIDI_Pipes.cpp"
    "MIDI_Interfaces/test-BLEMIDIPacketBuilder.cpp"
    "MIDI_Interfaces/test-BLEAPI.cpp"
    "MIDI_Interfaces/test-USBBulk.cpp"
    "Banks/test-Banks.cpp"
    "Selectors/test-ManyButtonsSelector.cpp"
    "Selectors/test-IncrementDecrementSelector.cpp"
    "Selectors/test-IncrementSelector.cpp"
)
target_include_directories(tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(tests
    PRIVATE Arduino_Helpers Control_Surface
    PRIVATE Arduino-Helpers::warnings)

# Add tests
gtest_discover_tests(tests DISCOVERY_TIMEOUT 60 TIMEOUT 20)
add_executable(Arduino-Helpers::tests ALIAS tests)

add_subdirectory(tools)